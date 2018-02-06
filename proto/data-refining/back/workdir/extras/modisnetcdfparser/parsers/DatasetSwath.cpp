#include "DatasetSwath.h"

#include <limits>

#include <netcdf.h>

#include "logger.h"
#include "square.h"
#include "testHit.h"
#include "OGRReader.h"
#include "readVariable.h"
#include "interpolation.h"
#include "VectorLayerShapeType.h"

#define PACKET 64*1024*1024
#define CHECK(e,t) if (!checkNCError(e,t)) return false;

DatasetSwath::DatasetSwath() : DatasetAbstract()
{
    rootid = 0;
}

DatasetSwath::~DatasetSwath()
{

}

bool DatasetSwath::parse(const std::string &inpath, const std::string &areaURI, const std::string &outpath)
{
    // https://oceancolor.gsfc.nasa.gov/docs/technical/ocean_level-2_data_products.pdf
    // 4.2.1 Retrieved Geophysical Parameters
    // All parameter SDSs have dimensions Number of Scan Lines x Pixels per Scan Line

    if (rootid)
        nc_close(rootid);
    clear();

    int status = nc_open(inpath.c_str(), NC_NOWRITE, &rootid);
    CHECK(status, "");

    size_t hit = 0;
    size_t bytes = 0;

    std::vector<double> dataLongitude, dataLatitude;
    std::vector<size_t> dimensions;
    {
        std::vector<size_t> dimtest;
        readVariable<double>(rootid, locLongitude, &dataLongitude, &dimensions);
        readVariable<double>(rootid, locLatitude, &dataLatitude, &dimtest);
        if (dimensions.size()!=dimtest.size())
        {
            LOG( "geo dimensions count mismatch");
            nc_close(rootid);
            return false;
        }
        for (size_t i = 0; i < dimensions.size(); ++i)
            if (dimensions[i]!=dimtest[i])
            {
                LOG( "geo dimension " << i << " size mismatch");
                nc_close(rootid);
                return false;
            }
    }

    double ncminlon = std::numeric_limits<double>::max();
    double ncmaxlon = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < dataLongitude.size(); ++i)
    {
        ncminlon = dataLongitude[i] < ncminlon ? dataLongitude[i] : ncminlon;
        ncmaxlon = dataLongitude[i] > ncmaxlon ? dataLongitude[i] : ncmaxlon;
    }
    LOG( "Longitude [" << ncminlon << " .. " << ncmaxlon << "]");
    double ncminlat = std::numeric_limits<double>::max();
    double ncmaxlat = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < dataLongitude.size(); ++i)
    {
        ncminlat = dataLatitude[i] < ncminlat ? dataLatitude[i] : ncminlat;
        ncmaxlat = dataLatitude[i] > ncmaxlat ? dataLatitude[i] : ncmaxlat;
    }
    LOG( "Latitude [" << ncminlat << " .. " << ncmaxlat << "]");

    // fast test
    std::vector<VectorLayerShapeType> shapes = ReadOGRDataSourceToShapes(areaURI.c_str());
    {
        double S = square(shapes, 1000);
        LOG("Square(m^2)="<<S);
        LOG("Estimated points="<<(size_t)(S/(resolution*resolution)));
    }

    double shminlon = std::numeric_limits<double>::max(); double shmaxlon = std::numeric_limits<double>::lowest();
    double shminlat = std::numeric_limits<double>::max(); double shmaxlat = std::numeric_limits<double>::lowest();
    for (size_t i = 0; i < shapes.size(); ++i)
    {
        shminlon = shapes[i].min.x < shminlon ? shapes[i].min.x : shminlon;
        shmaxlon = shapes[i].max.x > shmaxlon ? shapes[i].max.x : shmaxlon;
        shminlat = shapes[i].min.y < shminlat ? shapes[i].min.y : shminlat;
        shmaxlat = shapes[i].max.y > shmaxlat ? shapes[i].max.y : shmaxlat;
    }
    if ((shminlon > ncmaxlon)|(shminlat > ncmaxlat)|(shmaxlon < ncminlon)|(shmaxlat < ncminlat))
    {
        LOG("Result=Miss");
        nc_close(rootid);
        return true;
    }

    std::vector<double> dataValue;
    {
        std::vector<size_t> dimtest;
        {
            readVariable<double>(rootid, locValue, &dataValue, &dimtest);
            fixValues(&dataValue);
            fixDimensions(&dimensions, &dimtest);

            if (dimensions.size()!=dimtest.size())
            {
                LOG( "dimensions count mismatch" );
                nc_close(rootid);
                return false;
            }
            for (size_t i = 0; i < dimensions.size(); ++i)
                if (dimensions[i]!=dimtest[i])
                {
                    LOG( "dimension " << i << " size mismatch" );
                    nc_close(rootid);
                    return false;
                }

            for (size_t i = 0; i < dataValue.size(); ++i)
            {
                component2d pnt{dataLongitude[i], dataLatitude[i]};
                if (!std::isnan(dataValue[i]))
                {
                    if (testHit(shapes, pnt))
                        ++hit;
                    else
                        dataValue[i] = std::numeric_limits<double>::quiet_NaN();
                }
            }
            if (!hit)
            {
                LOG("Result=Miss");
                nc_close(rootid);
                return true;
            }
        }
    }
    // read spatial resolution
    nc_close(rootid);
    rootid = 0;

    // now form a string (possibly huge)
    outbuffer.clear();
    outbuffer.reserve(PACKET+1024);
    outbuffer = '#' + locLongitude.back() + '/' + locLatitude.back() + '/';
    if (locTime.empty())
        outbuffer += "/NoTimeData";
    else
        outbuffer += '/' + locTime.back();
    outbuffer += "/NoDepthData;\n";

    FILE *outputfile = fopen(outpath.c_str(), "wb");
    if (!outputfile)
    {
        LOG("Failed to open/create output file " << outpath);
        return false;
    }

    const int w = dimensions[1];
    const int h = dimensions[0];
    std::string timestr;
    for (int line = 0; line < (h-1); ++line)
    {
        if (!checkTime(line, &timestr))
            continue;
        timestr = '/' + timestr + "/undefined;\n";
        for (int pixel = 0; pixel < (w-1); ++pixel)
        {
            const int i = line*w+pixel;
            double v00 = dataValue[i];
            double v01 = dataValue[i+1];
            double v10 = dataValue[i+w];
            double v11 = dataValue[i+w+1];
            if (std::isnan(v00)|std::isnan(v01)|std::isnan(v10)|std::isnan(v11))
                continue;

            component2d p00{dataLongitude[i], dataLatitude[i]};
            component2d p01{dataLongitude[i+1], dataLatitude[i+1]};
            component2d p10{dataLongitude[i+w], dataLatitude[i+w]};
            component2d p11{dataLongitude[i+w+1], dataLatitude[i+w+1]};

            p00.x = p00.x < -180.0 ? p00.x + 360.0 : p00.x;
            p01.x = p01.x < -180.0 ? p01.x + 360.0 : p01.x;
            p10.x = p10.x < -180.0 ? p10.x + 360.0 : p10.x;
            p11.x = p11.x < -180.0 ? p11.x + 360.0 : p11.x;

            p00.x = p00.x > 180.0 ? p00.x - 360.0 : p00.x;
            p01.x = p01.x > 180.0 ? p01.x - 360.0 : p01.x;
            p10.x = p10.x > 180.0 ? p10.x - 360.0 : p10.x;
            p11.x = p11.x > 180.0 ? p11.x - 360.0 : p11.x;

            {
                //if (i == 4587)
                    //std::cout << "breakpoint" << std::endl;
                double fix = (p00.x >= 0) ? 360.0 : -360.0;
                component3d d1{p01.x - p00.x, p10.x - p00.x, p11.x - p00.x};
                component3d d2 = absof(d1 + fix);
                d1 = absof(d1);
                p01.x += d2.x < d1.x ? fix : 0.0;
                p10.x += d2.y < d1.y ? fix : 0.0;
                p11.x += d2.z < d1.z ? fix : 0.0;
            }

            p00.x = p00.x < -180.0 ? p00.x + 360.0 : p00.x;
            p01.x = p01.x < -180.0 ? p01.x + 360.0 : p01.x;
            p10.x = p10.x < -180.0 ? p10.x + 360.0 : p10.x;
            p11.x = p11.x < -180.0 ? p11.x + 360.0 : p11.x;

            p00.x = p00.x > 180.0 ? p00.x - 360.0 : p00.x;
            p01.x = p01.x > 180.0 ? p01.x - 360.0 : p01.x;
            p10.x = p10.x > 180.0 ? p10.x - 360.0 : p10.x;
            p11.x = p11.x > 180.0 ? p11.x - 360.0 : p11.x;


            double distX = distance(p00, p01);
            distX = std::max(distX, distance(p10, p11));
            double distY = distance(p00, p10);
            distY = std::max(distY, distance(p01, p11));

            if ((distX < 1.0)&(distY < 1.0))
            {
                outbuffer+=std::to_string(p00.x)+'/'+std::to_string(p00.y)+'/'+std::to_string(dataValue[i])+";\n";
                continue;
            }
            if ((distX < 1.4)&(distY < 1.4))
            {
                outbuffer+=std::to_string(p00.x)+'/'+std::to_string(p00.y)+'/'+std::to_string(v00)+"/undefined/undefined;\n";
                component2d O = (p00+p01+p10+p11)*0.25;
                double val = interpolate_quad_by_Phrogz_and_clamp(p00, p01, p10, p11, O,
                                                                  v00, v01, v10, v11);
                outbuffer+=std::to_string(O.x)+'/'+std::to_string(O.y)+'/'+std::to_string(val)+"/undefined/undefined;\n";
                ++hit;
                continue;
            }

            double stepC1 = resolution / distX;
            double stepC2 = resolution / distY;

            for (double c1 = 0; c1 < 1.0; c1 += stepC1)
            {
                for (double c2 = 0; c2 < 1.0; c2 += stepC2)
                {
                    component2d O = interpolate_from_even_quad<component2d>(p00, p10, p01, p11, c1, c2);
                    double val = interpolate_quad_by_Phrogz_and_clamp(p00, p01, p10, p11, O,
                                                                      v00, v01, v10, v11);
                    outbuffer+=std::to_string(O.x)+'/'+std::to_string(O.y)+'/'+std::to_string(val)+timestr;
                    ++hit;
                }
            }
            --hit;

            if (outbuffer.size() >= PACKET)
            {
                size_t pack = fwrite(outbuffer.c_str(), 1, outbuffer.size(), outputfile);
                bytes += pack;
                if (outbuffer.size() != pack)
                {
                    LOG("Result=Error on File Write");
                    LOG("Count=" << hit);
                    fclose(outputfile);
                    return true;
                }
                outbuffer.clear();
            }
        }
    }

    if (!outbuffer.empty())
    {
        size_t pack = fwrite(outbuffer.c_str(), 1, outbuffer.size(), outputfile);
        bytes += pack;
        if (outbuffer.size() != pack)
        {
            LOG("Result=Error on File Write");
            LOG("Count=" << hit);
            fclose(outputfile);
            return true;
        }
        outbuffer.clear();
    }
    fclose(outputfile);

    LOG(     "Total output size: "
              << (bytes > (1024*1024) ? bytes / (1024*1024)
                                      : (bytes > 1024 ? bytes / 1024
                                                       : bytes))
              << (bytes > (1024*1024) ? "MB"
                                      : (bytes > 1024 ? "KB"
                                                      : "B")) );

    LOG("Count=" << hit);
    LOG("Result=Hit");
}

bool DatasetSwath::checkTime(const int, std::string *const ascii)
{
    if (ascii)
        *ascii = "undefined";
    return true;
}

bool DatasetSwath::checkNCError(int e, const std::string &t)
{
    if (e!=NC_NOERR)
    {
        LOG("Error: " << nc_strerror(e)
            <<"\nhint: "<<t
            <<"\n"<<"Result=Failure");
        if (rootid)
            nc_close(rootid);
        return false;
    }
    return true;
}

void DatasetSwath::fixValues(std::vector<double> *const)
{
    // some datasets have undescribed by NetCDF structure special values (like NO_DATA)
    // but by default readVariable should return correct one;
    return;
}

void DatasetSwath::fixDimensions(std::vector<size_t> *const __restrict , std::vector<size_t> *const __restrict )
{
    // for some reasons some dimensions are totally undescribed and equals to one
    return;
}

