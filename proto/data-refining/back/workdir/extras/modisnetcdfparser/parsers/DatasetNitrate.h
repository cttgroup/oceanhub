#ifndef DATASETNITRATE_H
#define DATASETNITRATE_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetNitrate : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetNitrate();
    virtual ~DatasetNitrate();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETNITRATE_H
