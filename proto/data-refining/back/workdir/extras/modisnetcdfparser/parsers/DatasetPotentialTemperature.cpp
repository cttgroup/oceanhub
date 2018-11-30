#include "DatasetPotentialTemperature.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetPotentialTemperature::DatasetPotentialTemperature() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("thetao");
    locTime.push_back("time");
    locDepth.push_back("depth");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetPotentialTemperature::~DatasetPotentialTemperature()
{

}

void DatasetPotentialTemperature::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
