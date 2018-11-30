#include "DatasetIceThickness.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetIceThickness::DatasetIceThickness() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("sithick");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetIceThickness::~DatasetIceThickness()
{

}

void DatasetIceThickness::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
