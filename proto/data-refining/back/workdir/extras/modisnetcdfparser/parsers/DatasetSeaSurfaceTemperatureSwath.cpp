#include "DatasetSeaSurfaceTemperatureSwath.h"

DatasetSeaSurfaceTemperatureSwath::DatasetSeaSurfaceTemperatureSwath() : DatasetSwath()
{
    locValue.push_back("geophysical_data"); locValue.push_back("sst");
    locLongitude.push_back("navigation_data"); locLongitude.push_back("longitude");
    locLatitude.push_back("navigation_data"); locLatitude.push_back("latitude");
}

DatasetSeaSurfaceTemperatureSwath::~DatasetSeaSurfaceTemperatureSwath()
{

}
