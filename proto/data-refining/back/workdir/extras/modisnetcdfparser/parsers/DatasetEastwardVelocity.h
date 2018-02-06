#ifndef DATASETEASTWARDVELOCITY_H
#define DATASETEASTWARDVELOCITY_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetEastwardVelocity : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetEastwardVelocity();
    virtual ~DatasetEastwardVelocity();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETEASTWARDVELOCITY_H
