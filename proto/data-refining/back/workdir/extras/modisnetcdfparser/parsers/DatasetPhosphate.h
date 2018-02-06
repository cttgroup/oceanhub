#ifndef DATASETPHOSPHATE_H
#define DATASETPHOSPHATE_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetPhosphate : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetPhosphate();
    virtual ~DatasetPhosphate();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETPHOSPHATE_H
