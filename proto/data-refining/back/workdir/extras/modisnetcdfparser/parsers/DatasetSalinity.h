#ifndef DATASETSALINITY_H
#define DATASETSALINITY_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetSalinity : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetSalinity();
    virtual ~DatasetSalinity();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETSALINITY_H
