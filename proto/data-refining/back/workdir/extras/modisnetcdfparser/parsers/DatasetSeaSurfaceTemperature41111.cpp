#include "DatasetSeaSurfaceTemperature41111.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetSeaSurfaceTemperature41111::DatasetSeaSurfaceTemperature41111() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("CHL");
    locTime.push_back("time_counter");
    locDepth.push_back("depth");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetSeaSurfaceTemperature41111::~DatasetSeaSurfaceTemperature41111()
{

}

void DatasetSeaSurfaceTemperature41111::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
