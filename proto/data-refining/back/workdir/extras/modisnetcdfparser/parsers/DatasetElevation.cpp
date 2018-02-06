#include "DatasetElevation.h"

DatasetElevation::DatasetElevation() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("elevation");
    locLongitude.push_back("lon");
    locLatitude.push_back("lat");
}

DatasetElevation::~DatasetElevation()
{

}
