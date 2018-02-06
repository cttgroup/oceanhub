#ifndef DATASETHEIGHTABOVEGEOID_H
#define DATASETHEIGHTABOVEGEOID_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetHeightAboveGeoid : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetHeightAboveGeoid();
    virtual ~DatasetHeightAboveGeoid();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETHEIGHTABOVEGEOID_H
