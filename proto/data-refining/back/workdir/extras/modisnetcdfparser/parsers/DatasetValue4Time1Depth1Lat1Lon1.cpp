#include "DatasetValue4Time1Depth1Lat1Lon1.h"

#include <cstdio>
#include <limits>

#include <netcdf.h>

#include "readVariable.h"
#include "OGRReader.h"
#include "VectorLayerShapeType.h"
#include "logger.h"
#include "timefun.h"
#include "coordcvt.h"
#include "interpolation.h"
#include "testHit.h"

#define PACKET 64*1024*1024
#define TIME_OF_DAY 12

#define CHECK(e,t) if (!checkNCError(e,t)) return false;

DatasetValue4Time1Depth1Lat1Lon1::DatasetValue4Time1Depth1Lat1Lon1() : DatasetAbstract()
{
    rootid = 0;
}

DatasetValue4Time1Depth1Lat1Lon1::~DatasetValue4Time1Depth1Lat1Lon1()
{

}

bool DatasetValue4Time1Depth1Lat1Lon1::parse(const std::string &inpath, const std::string& areaURI, const std::string& outpath)
{
    if (locLatitude.empty() | locLongitude.empty())
    {
        LOG("Geo dataset\'s locations are unset");
        return false;
    }
    clear();

    size_t hit = 0;
    size_t bytes = 0;

    bool useTime = !locTime.empty();
    bool useDepth = !locDepth.empty();

    std::vector<size_t> dimensions;
    dimensions.reserve(4);
    size_t dimTime = 1;
    size_t dimDepth = 1;
    size_t dimLon = 1;
    size_t dimLat = 1;

    if (rootid)
        nc_close(rootid);
    int status = nc_open(inpath.c_str(), NC_NOWRITE, &rootid);
    CHECK(status, "");

    std::vector<int64_t> dataTime;
    if (useTime)
    {
        std::vector<size_t> dimtest;
        if (NC_NOERR != readVariable<int64_t>(rootid, locTime, &dataTime, &dimtest))
        {
            LOG( "Failed to read time");
            nc_close(rootid);
            return false;
        }
        if (dimtest.size()!=1)
        {
            LOG( "time variable is not a vector");
            nc_close(rootid);
            return false;
        }
        cvtFileTimeToHrsSince1900(&dataTime);
        dimTime = dimtest.front();
        dimensions.push_back(dimtest.front());
    }

    std::vector<double> dataDepth;
    if (useDepth)
    {
        std::vector<size_t> dimtest;
        if (NC_NOERR != readVariable<double>(rootid, locDepth, &dataDepth, &dimtest))
        {
            LOG( "Failed to read depth");
            nc_close(rootid);
            return false;
        }
        if (dimtest.size()!=1)
        {
            LOG( "depth variable is not a vector");
            nc_close(rootid);
            return false;
        }
        dimDepth = dimtest.front();
        dimensions.push_back(dimtest.front());
    }

    std::vector<double> dataLatitude;
    {
        std::vector<size_t> dimtest;
        if (NC_NOERR != readVariable<double>(rootid, locLatitude, &dataLatitude, &dimtest))
        {
            LOG( "Failed to read Latitude");
            nc_close(rootid);
            return false;
        }
        if (dimtest.size()!=1)
        {
            LOG( "Latitude variable is not a vector");
            nc_close(rootid);
            return false;
        }
        dimLat = dimtest.front();
        dimensions.push_back(dimtest.front());
    }

    std::vector<double> dataLongitude;
    {
        std::vector<size_t> dimtest;
        if (NC_NOERR != readVariable<double>(rootid, locLongitude, &dataLongitude, &dimtest))
        {
            LOG( "Failed to read Longitude");
            nc_close(rootid);
            return false;
        }
        if (dimtest.size()!=1)
        {
            LOG( "Longitude variable is not a vector");
            nc_close(rootid);
            return false;
        }
        dimLon = dimtest.front();
        dimensions.push_back(dimtest.front());
    }

    double ncminlon = std::min(dataLongitude.front(), dataLongitude.back());
    double ncminlat = std::min(dataLatitude.front(), dataLatitude.back());
    double ncmaxlon = std::max(dataLongitude.front(), dataLongitude.back());
    double ncmaxlat = std::max(dataLatitude.front(), dataLatitude.back());
    LOG( "Longitude [" << ncminlon << " .. " << ncmaxlon << "]");
    LOG( "Latitude [" << ncminlat << " .. " << ncmaxlat << "]");

    // fast test
    std::vector<VectorLayerShapeType> shapes = ReadOGRDataSourceToShapes(areaURI.c_str());
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

    bool ac = dataLongitude.back() > dataLongitude.front();
    for (size_t i = 0; i < dataLongitude.size()-1; ++i)
        if ((dataLongitude[i+1] > dataLongitude[i])!=ac)
        {
            LOG("Result=Error Longitude array is not ascending or descending");
            nc_close(rootid);
            return 0;
        }
    ac = dataLatitude.back() > dataLatitude.front();
    for (size_t i = 0; i < dataLatitude.size()-1; ++i)
        if ((dataLatitude[i+1] > dataLatitude[i])!=ac)
        {
            LOG("Result=Error Longitude array is not ascending or descending");
            nc_close(rootid);
            return 0;
        }

    std::vector<double> dataValues;
    {
        std::vector<size_t> dimtest;
        readVariable<double>(rootid, locValue, &dataValues, &dimtest);
        if (dimensions.size()!=dimtest.size())
        {
            {
                LOG( "dimensions count mismatch" );
                nc_close(rootid);
                return 0;
            }
        }
        for (size_t i = 0; i < dimensions.size(); ++i)
            if (dimensions[i]!=dimtest[i])
            {
                LOG( "dimension " << i << " size mismatch" );
                nc_close(rootid);
                return 0;
            }
    }


    // going by latitude first to determine longitude degree size
    constexpr double EarthPolarRadius = 6356800;
    const double LatDegPer1Res = 180.0*resolution/EarthPolarRadius; // there is more precise way, but we need constant step
    const int ylast = dataLatitude.size() - 1;
    const int xlast = dataLongitude.size() - 1;

    std::vector<size_t> offsetList;
    if (dataTime.empty()) // also if useTime set
    {
        if ((till1900H>=0)||(since1900H>=0))
        {
            LOG("Error=Time data is absent, but <since> and/or <till> options are set");
            return false;
        }
        else
        {
            // this means we make zero offset by time (works even if no time dimension used)
            offsetList.assign(1, 0);
            dataTime.resize(1);
            dataTime[0] = -1;
        }

    }
    else
    {
        std::set<int> days;
        for (size_t t = 0; t < dataTime.size(); ++t)
        {
            int64_t hrs = dataTime[t];
            if ((hrs >= since1900H)&(hrs <= till1900H))
                days.insert(dataTime[t]/24);
        }
        offsetList.reserve(days.size());
        for (auto it = days.cbegin(); it != days.cend(); ++it)
        {
            const int64_t dayTime = (*it)*24 + TIME_OF_DAY;
            size_t minTimeDiffPos = 0;
            int minTimeDiff = 48;
            for (size_t t = 0; t < dataTime.size(); ++t)
            {
                int64_t timeDiff = dataTime[t] - dayTime;
                timeDiff = timeDiff < 0 ? -timeDiff : timeDiff;
                if (timeDiff < minTimeDiff)
                {
                    minTimeDiff = timeDiff;
                    minTimeDiffPos = t;
                    if (timeDiff == 0)
                        break;
                }
            }
            if (minTimeDiff < 24)
                offsetList.push_back(minTimeDiffPos);
        }
    }
    // first dimension in vector is track
    // second is cross

    outbuffer.clear();
    FILE *outputfile = fopen(outpath.c_str(), "wb");
    if (!outputfile)
    {
        LOG("Failed to open/create output file " << outpath);
        return false;
    }


    const size_t dimLonLat = dimLon*dimLat;
    outbuffer.reserve(offsetList.size() * dimDepth * dimLonLat * 32);
    outbuffer = '#' + locLongitude.back() + '/' + locLatitude.back() + '/' + locValue.back();
    if (useTime)
        outbuffer += '/' + locTime.back();
    else
        outbuffer += "/NoTimeData";
    if (useDepth)
        outbuffer += '/' + locDepth.back();
    else
        outbuffer += "/NoDepthData";
    outbuffer += ";\n";

    for (size_t l = 0; l < offsetList.size(); ++l)
    {
        size_t offset = offsetList[l]*dimDepth*dimLat*dimLon;
        int64_t timestamp = dataTime[offsetList[l]]; // time is majorest dimension
        const std::string timestr = '/'+hoursSince1900ToAscii(timestamp)+"/";

        for (size_t di = 0; di < dimDepth; ++di)
        {
            const std::string timedepthstr = timestr + (useDepth ? std::to_string(dataDepth[di]) : std::string("undefined")) + ";\n";

            for (double lat = ncminlat; lat < ncmaxlat; lat += LatDegPer1Res)
            {
                int y = 0;
                while ((y < ylast) &&
                       (lat > dataLatitude[y+1])==(lat > dataLatitude[y]))
                {
                    ++y;
                }
                if (y >= ylast)
                    continue;
                std::string latstr = '/'+std::to_string(lat)+'/';
                double d2 = (lat - dataLatitude[y])/(dataLatitude[y+1] - dataLatitude[y]);

                double LonDegPer1Res = resolution/MetersPerLongitudeDegreeAtLatitude(lat);
                for (double lon = ncminlon; lon < ncmaxlon; lon += LonDegPer1Res)
                {
                    int x = 0;
                    while ((x < xlast) &&
                           (lon > dataLongitude[x+1])==(lon > dataLongitude[x]))
                    {
                        ++x;
                    }
                    if (x >= xlast)
                        continue;

                    const double v00 = dataValues[offset + di*dimLonLat + y*dimLon + x];
                    const double v01 = dataValues[offset + di*dimLonLat + y*dimLon + x+1];
                    const double v10 = dataValues[offset + di*dimLonLat + (y+1)*dimLon + x];
                    const double v11 = dataValues[offset + di*dimLonLat + (y+1)*dimLon + x+1];

                    if (std::isnan(v00)|std::isnan(v01)|std::isnan(v10)|std::isnan(v11))
                        continue;

                    if (testHit(shapes, component2d{lon, lat}))
                    {
                        double d1 = (lon - dataLongitude[x])/(dataLongitude[x+1] - dataLongitude[x]);
                        double val = interpolate_from_even_quad(v00, v10,
                                                                v01, v11,
                                                                 d1,  d2);
                        outbuffer+=std::to_string(lon)+latstr+std::to_string(val)+timedepthstr;
                        ++hit;
                    }

                    if (outbuffer.size() >= PACKET)
                    {
                        size_t pack = fwrite(outbuffer.c_str(), 1, outbuffer.size(), outputfile);
                        bytes += pack;
                        if (outbuffer.size() != pack)
                        {
                            LOG("Result=Error on File Write");
                            LOG("Count=" << hit);
                            fclose(outputfile);
                            return false;
                        }
                        outbuffer.clear();
                    }
                }
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
            return false;
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

    return true;
}

bool DatasetValue4Time1Depth1Lat1Lon1::checkNCError(int e, const std::string &t)
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

void DatasetValue4Time1Depth1Lat1Lon1::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    return; //default is unchanged
}
