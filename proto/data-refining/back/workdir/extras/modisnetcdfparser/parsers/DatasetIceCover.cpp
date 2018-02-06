#include "DatasetIceCover.h"

DatasetIceCover::DatasetIceCover() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("ci");
    locLongitude.push_back("longitude");
    locLatitude.push_back("latitude");
    locTime.push_back("time");
}

DatasetIceCover::~DatasetIceCover()
{

}
