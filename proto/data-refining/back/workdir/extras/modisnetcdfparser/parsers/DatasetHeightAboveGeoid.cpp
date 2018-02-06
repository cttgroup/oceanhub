#include "DatasetHeightAboveGeoid.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetHeightAboveGeoid::DatasetHeightAboveGeoid() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("zos");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetHeightAboveGeoid::~DatasetHeightAboveGeoid()
{

}

void DatasetHeightAboveGeoid::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
