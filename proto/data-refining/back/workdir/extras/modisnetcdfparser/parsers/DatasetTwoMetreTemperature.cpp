#include "DatasetTwoMetreTemperature.h"

DatasetTwoMetreTemperature::DatasetTwoMetreTemperature() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("t2m");
    locLongitude.push_back("longitude");
    locLatitude.push_back("latitude");
    locTime.push_back("time");
}

DatasetTwoMetreTemperature::~DatasetTwoMetreTemperature()
{

}
