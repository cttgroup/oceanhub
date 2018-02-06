#include "DatasetPotentialSeaFloorTemperature.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetPotentialSeaFloorTemperature::DatasetPotentialSeaFloorTemperature() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("bottomT");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetPotentialSeaFloorTemperature::~DatasetPotentialSeaFloorTemperature()
{

}

void DatasetPotentialSeaFloorTemperature::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
