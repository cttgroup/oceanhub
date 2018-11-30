#include "DatasetDensityOceanMixedLayerThickness.h"

#include <cstdint>
#include <vector>

#include "timefun.h"

DatasetDensityOceanMixedLayerThickness::DatasetDensityOceanMixedLayerThickness() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("mlotst");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetDensityOceanMixedLayerThickness::~DatasetDensityOceanMixedLayerThickness()
{

}

void DatasetDensityOceanMixedLayerThickness::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1950ToHoursSince1900((*timedata)[i]);
    return;
}
