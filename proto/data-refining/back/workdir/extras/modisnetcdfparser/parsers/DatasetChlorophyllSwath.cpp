#include "DatasetChlorophyllSwath.h"

DatasetChlorophyllSwath::DatasetChlorophyllSwath() : DatasetSwath()
{
    locValue.push_back("geophysical_data"); locValue.push_back("chlor_a");
    locLongitude.push_back("navigation_data"); locLongitude.push_back("longitude");
    locLatitude.push_back("navigation_data"); locLatitude.push_back("latitude");
}

DatasetChlorophyllSwath::~DatasetChlorophyllSwath()
{

}
