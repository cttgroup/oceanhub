#ifndef DATASETNORTHWARDVECLOCITY_H
#define DATASETNORTHWARDVECLOCITY_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetNorthwardVeclocity : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetNorthwardVeclocity();
    virtual ~DatasetNorthwardVeclocity();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETNORTHWARDVECLOCITY_H
