#include "DatasetAbstract.h"

#include "logger.h"
#include "timefun.h"

DatasetAbstract::DatasetAbstract()
{
    since1900H = -1;
    till1900H = -1;
    resolution = 1000.0;
}

DatasetAbstract::~DatasetAbstract()
{

}

bool DatasetAbstract::setUtcTimeSince(const std::string &ascii)
{
    if (!ascii.empty())
        since1900H = asciiToHoursSince1900(ascii);
    if (since1900H == -1)
    {
        LOG("Error=failed to parse since time");
        return false;
    }
    return true;
}

bool DatasetAbstract::setUtcTimeTill(const std::string &ascii)
{
    if (!ascii.empty())
        till1900H = asciiToHoursSince1900(ascii);
    if (till1900H == -1)
    {
        LOG("Error=failed to parse till time");
        return false;
    }
    return true;
}

void DatasetAbstract::setResolution(const double res)
{
    resolution = res;
}

void DatasetAbstract::clear()
{
    return;
}
