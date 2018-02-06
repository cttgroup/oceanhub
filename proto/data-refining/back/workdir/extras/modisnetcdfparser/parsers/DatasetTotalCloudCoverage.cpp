#include "DatasetTotalCloudCoverage.h"

DatasetTotalCloudCoverage::DatasetTotalCloudCoverage() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("tcc");
    locLongitude.push_back("longitude");
    locLatitude.push_back("latitude");
    locTime.push_back("time");
}

DatasetTotalCloudCoverage::~DatasetTotalCloudCoverage()
{

}
