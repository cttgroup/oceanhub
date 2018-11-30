#include "DatasetEastwardVelocity.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetEastwardVelocity::DatasetEastwardVelocity() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("uo");
    locTime.push_back("time");
    locDepth.push_back("depth");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetEastwardVelocity::~DatasetEastwardVelocity()
{

}

void DatasetEastwardVelocity::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
