#include "DatasetPhosphate.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetPhosphate::DatasetPhosphate() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("PO4");
    locTime.push_back("time_counter");
    locDepth.push_back("depth");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetPhosphate::~DatasetPhosphate()
{

}

void DatasetPhosphate::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
