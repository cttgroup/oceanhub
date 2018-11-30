#ifndef DATASETICENORTHWARDVELOCITY_H
#define DATASETICENORTHWARDVELOCITY_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetIceNorthwardVelocity : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetIceNorthwardVelocity();
    virtual ~DatasetIceNorthwardVelocity();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETICENORTHWARDVELOCITY_H
