#include "DatasetIceConcentration.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetIceConcentration::DatasetIceConcentration() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("siconc");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetIceConcentration::~DatasetIceConcentration()
{

}

void DatasetIceConcentration::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
