#ifndef DATASETSILICATE_H
#define DATASETSILICATE_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetSilicate : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetSilicate();
    virtual ~DatasetSilicate();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *timedata) const override;
};

#endif // DATASETSILICATE_H
