#include "DatasetOxygen.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetOxygen::DatasetOxygen() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("O2");
    locTime.push_back("time_counter");
    locDepth.push_back("depth");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetOxygen::~DatasetOxygen()
{

}

void DatasetOxygen::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
