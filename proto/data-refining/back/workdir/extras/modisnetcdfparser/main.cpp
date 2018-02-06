#include <cstring>
#include <cfloat>
#include <ctime>
#include <cstdio>

#include <chrono>
#include <iostream>
#include <set>
#include <vector>
#include <netcdf.h>

#include "OGRReader.h"
#include "VectorLayerShapeType.h"
#include "testHit.h"
#include "GdalTransform.h"
#include "stringsplit.h"
#include "coordcvt.h"
#include "readVariable.h"
#include "interpolation.h"
#include "square.h"
#include "logger.h"
#include "timefun.h"

#include "parsers/DatasetAbstract.h"
#include "parsers/DatasetIron.h"
#include "parsers/DatasetCarbon.h"
#include "parsers/DatasetOxygen.h"
#include "parsers/DatasetNitrate.h"
#include "parsers/DatasetSalinity.h"
#include "parsers/DatasetSilicate.h"
#include "parsers/DatasetIceCover.h"
#include "parsers/DatasetPhosphate.h"
#include "parsers/DatasetWindSpeed.h"
#include "parsers/DatasetElevation.h"
#include "parsers/DatasetIceThickness.h"
#include "parsers/DatasetEastwardVelocity.h"
#include "parsers/DatasetHeightAboveGeoid.h"
#include "parsers/DatasetChlorophyllSwath.h"
#include "parsers/DatasetChlorophyll41111.h"
#include "parsers/DatasetIceConcentration.h"
#include "parsers/DatasetNorthwardVeclocity.h"
#include "parsers/DatasetTotalCloudCoverage.h"
#include "parsers/DatasetIceEastwardVelocity.h"
#include "parsers/DatasetTwoMetreTemperature.h"
#include "parsers/DatasetPhytoplanktonCarbon.h"
#include "parsers/DatasetIceNorthwardVelocity.h"
#include "parsers/DatasetPotentialTemperature.h"
#include "parsers/DatasetRadiativeAttenuation.h"
#include "parsers/DatasetSeaSurfaceTemperature41111.h"
#include "parsers/DatasetSeaSurfaceTemperatureSwath.h"
#include "parsers/DatasetPotentialSeaFloorTemperature.h"
#include "parsers/DatasetDensityOceanMixedLayerThickness.h"


#define PRECISION 1000

extern "C" {
size_t parseMODIS(const char *ncpath, const char *areaURI, const char *dset, const float spres,
                  std::string since, std::string till,
                  const char *output);
}

#ifndef BUILDLIB
static void printHelp();
static void printPresets();
static void printDatasets();
static bool parseOptions(int argc, char **argv,
                         std::string *ip,
                         std::string *dset,
                         std::string *sp,
                         std::string *op,
                         std::string *lp,
                         double *prec,
                         std::string *since,
                         std::string *till);
int main(int argc, char *argv[])
{
    //"/home/max/git/ocean/_data/characteristics/2_sea_surface_temperature.nc"

    std::string ip,op,sp,dset,lp,since,till;
    double prec = PRECISION;

    parseOptions(argc,argv,&ip,&dset,&sp,&op,&lp,&prec,&since,&till);
    if (ip.empty()|sp.empty()|op.empty())
    {
        std::cout << "options --input,--output,--shape are mandatory" << std::endl;
        return -1;
    }

    if ((!lp.empty())&&(LoggerStream::getInstance()))
        LoggerStream::getInstance()->init(lp);

    // /home/max/projects/files/Water/20160901-20160930-mean-diffuse.nc
    // /home/max/projects/files/Water/bathymetric.nc
    // /home/max/projects/files/Water/kislorod_dataset-global-analysis-forecast-bio-001-014_1491320452213.nc
    // /home/max/projects/files/Water/sept2016.nc
    // /home/max/projects/files/Water/solenie_global-analysis-forecast-phy-001-024_1491320064980.nc

    size_t count = parseMODIS(ip.c_str(), sp.c_str(), dset.c_str(), prec, since, till, op.c_str());
    // <longitude>/<latitude>/<sst>;\n
    // ...
    return 0;
}
#endif

enum class DATAKIND { UNDEFINED, SST, CHLOR, WIND, ELEV, CI, TCC, T2M } ;

size_t parseMODIS(const char *ncpath,
                  const char *areaURI,
                  const char *dset,
                  const float res,
                  std::string since, std::string till,
                  const char *output)
{
    LOG(   "ncpath:" << ncpath
              << "\nareaURI:" << areaURI
              << "\ndataset:" << dset
              << "\nres:" << res);

    DatasetAbstract *pDSet = NULL;
    if (0==strcmp(dset, "sst"))
    {
        pDSet = new DatasetSeaSurfaceTemperatureSwath;
        // no time
    }
    else if (0==strncmp(dset, "chlor", 5))
    {
        pDSet = new DatasetChlorophyllSwath;
        // no time
    }
    else if (0==strncmp(dset, "elev", 4))
    {
        pDSet = new DatasetElevation;
        // no time
    }
    else if (0==strcmp(dset, "wind"))
    {
        pDSet = new DatasetWindSpeed;
        // no time
    }
    else if (0==strcmp(dset, "ci"))
    {
        pDSet = new DatasetIceCover;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "tcc"))
    {
        pDSet = new DatasetTotalCloudCoverage;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "t2m"))
    {
        pDSet = new DatasetTwoMetreTemperature;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "pp"))
    {
        pDSet = new DatasetCarbon;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "po4"))
    {
        pDSet = new DatasetPhosphate;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "O2"))
    {
        pDSet = new DatasetOxygen;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "Fe"))
    {
        pDSet = new DatasetIron;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "NO3"))
    {
        pDSet = new DatasetNitrate;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "Si"))
    {
        pDSet = new DatasetSilicate;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "phyc"))
    {
        pDSet = new DatasetPhytoplanktonCarbon;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "so"))
    {
        pDSet = new DatasetSalinity;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "sithick"))
    {
        pDSet = new DatasetIceThickness;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "thetao"))
    {
        pDSet = new DatasetPotentialTemperature;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "siconc"))
    {
        pDSet = new DatasetIceConcentration;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "uo"))
    {
        pDSet = new DatasetEastwardVelocity;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "usi"))
    {
        pDSet = new DatasetIceEastwardVelocity;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "bottomT"))
    {
        pDSet = new DatasetPotentialSeaFloorTemperature;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "vsi"))
    {
        pDSet = new DatasetIceNorthwardVelocity;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "zos"))
    {
        pDSet = new DatasetHeightAboveGeoid;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "mlotst"))
    {
        pDSet = new DatasetDensityOceanMixedLayerThickness;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "vo"))
    {
        pDSet = new DatasetNorthwardVeclocity;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else if (0==strcmp(dset, "KD490"))
    {
        pDSet = new DatasetRadiativeAttenuation;
        pDSet->setUtcTimeSince(since);
        pDSet->setUtcTimeTill(till);
    }
    else
    {
        LOG("unknown dataset " << dset);
        return 0;
    }

    std::chrono::steady_clock::time_point timeBegin = std::chrono::steady_clock::now();

    if (pDSet)
    {
        if (!pDSet->parse(ncpath, areaURI, output))
            LOG("errors during parsing...");
    }
    delete pDSet;

    std::chrono::steady_clock::time_point timeEnd= std::chrono::steady_clock::now();
    LOG( "Total Time = " << std::chrono::duration_cast<std::chrono::milliseconds>(timeEnd - timeBegin).count() << " ms");
    return 0;
}

#ifndef BUILDLIB
static void printHelp()
{
// --input=<path> --shape=<path> --output=<path> --precision=<float> --dataset=sst
    std::cout << "--input=/home/example/path.nc\n"
                 "--output=/home/example/anotherpath.txt\n"
                 "--logfile=/home/example/yetanotherpath.log\n"
                 "--shape=wkt://SRID=4326;POLYGON ... \n"
                 "   or\n"
                 "--shape=file:///home/example/onemorepath.shp\n"
                 "   or\n"
                 "--shape=preset://nameOfPreset                    see --list-presets\n"
                 "--dataset=sst                                    see --list-datasets\n"
                 "--precision=1000                                 reserved" << std::endl;

}

// --input=<path> --shape=<path> --output=<path> --precision=<float> --dataset=sst
static bool parseOptions(int argc, char **argv, std::string *ip, std::string *dset, std::string *sp, std::string *op, std::string *lp, double *prec,
                         std::string *since,
                         std::string *till)
{
    std::vector<std::string> optval;

    if (argc < 2)
        printHelp();
    for (int i = 0; i < argc; ++i)
    {
        optval = split(argv[i], '=');
        if (0==strcmp(argv[i], "--help"))
            printHelp();
        else if (0==strcmp(argv[i], "--list-presets"))
            printPresets();
        else if (0==strcmp(argv[i], "--list-datasets"))
            printDatasets();
        else
        {
            if (optval.size()<2)
                continue;
            if (optval.size()>2)
            {
                for (size_t j = 2; j < optval.size(); ++j)
                    optval[1]+='='+optval[j];
                optval.resize(2);
            }
            if (optval.front()=="--input")
                *ip = std::move(optval.back());
            else if (optval.front()=="--dataset")
                *dset = std::move(optval.back());
            else if (optval.front()=="--shape")
                *sp = std::move(optval.back());
            else if (optval.front()=="--output")
                *op = std::move(optval.back());
            else if (optval.front()=="--logfile")
                *lp = std::move(optval.back());
            else if (optval.front()=="--precision")
                *prec = atof(optval.back().c_str());
            else if (optval.front()=="--since")
                *since = optval.back();
            else if (optval.front()=="--till")
                *till = optval.back();
        }
    }
    return true;
}

void printPresets()
{
    std::cout << "----------------------------------------------------\n"
                 "Preset: world\n"
                 "wkt://SRID=4326;POLYGON ((-179 -89,179 -89,179 89,-179 89,-179 -89))\n"
                 "----------------------------------------------------\n"
                 "Preset: norv_sea\n"
                 "wkt://SRID=4326;MULTIPOLYGON (((5.897592 59.453594, -15.743695 64.412255, 15.617894 77.781198, 27.955703 70.515584, 5.897592 59.453594)), ((145.933452 43, 151.670932 43, 151.670932 39, 145.933452 39, 145.933452 43)))"
                 "----------------------------------------------------\n"
              << std::endl;
    return;
}

void printDatasets()
{
    std::cout << "Dataset options         | Description\n"
                 "  PP                    |   Carbon\n"
                 "  chlor                 |   Chlorophyll\n"
                 "  mlotst                |   Density Ocean Mixed Layer Thickness\n"
                 "  uo                    |   Eastward velocity\n"
                 "  elev                  |   Elevation (bathymetric)\n"
                 "  zos                   |   Height Above Geoid\n"
                 "  siconc                |   Ice Concentration\n"
                 "  ci                    |   Ice Cover\n"
                 "  usi                   |   Ice Eastward Velocity\n"
                 "  vsi                   |   Ice Northward Velocity\n"
                 "  sithick               |   Ice Thickness\n"
                 "  Fe                    |   Iron\n"
                 "  NO3                   |   Nitrate\n"
                 "  vo                    |   Northward Velocity\n"
                 "  O2                    |   Oxygen\n"
                 "  PO4                   |   Phosphate\n"
                 "  phyc                  |   Phytoplankton expressed in carbon\n"
                 "  bottomT               |   Potential temperature at sea floor\n"
                 "  thetao                |   Potential Temperature\n"
                 "  KD490                 |   Volume attenuation coefficient of downwelling radiative flux\n"
                 "  so                    |   Salinity\n"
                 "  sst                   |   Sea Surface Temperature\n"
                 "  t2m                   |   Two Metre Temperature\n"
                 "  tcc                   |   Total Cloud Coverage\n"
                 "  wind                  |   Wind Speed\n"
                 "  Si                    |   Silicate\n"
              << std::endl;
}

#endif

