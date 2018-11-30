#include "DatasetIceNorthwardVelocity.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetIceNorthwardVelocity::DatasetIceNorthwardVelocity() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("vsi");
    locTime.push_back("time_counter");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetIceNorthwardVelocity::~DatasetIceNorthwardVelocity()
{

}

void DatasetIceNorthwardVelocity::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
