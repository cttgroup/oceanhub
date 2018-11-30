#include "DatasetRadiativeAttenuation.h"

#include <cstdint>
#include <vector>

#include "timefun.h"
#include "logger.h"

DatasetRadiativeAttenuation::DatasetRadiativeAttenuation() : DatasetValue4Time1Depth1Lat1Lon1()
{
    locValue.push_back("KD490");
    locTime.push_back("time");
    locLatitude.push_back("latitude");
    locLongitude.push_back("longitude");
}

DatasetRadiativeAttenuation::~DatasetRadiativeAttenuation()
{

}

void DatasetRadiativeAttenuation::cvtFileTimeToHrsSince1900(std::vector<int64_t> * const timedata) const
{
    LOG("DatasetRadiativeAttenuation does not account leap seconds for simplicity");
    // input is in seconds since start of 1970 (gregorian)
    // up to 27sec (and may be couple more already) inaccuracy
    for (size_t i = 0; i < timedata->size(); ++i)
        (*timedata)[i] = hoursSince1970ToHoursSince1900((*timedata)[i] / 3600);
    return;
}
