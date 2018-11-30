#ifndef DATASETOXYGEN_H
#define DATASETOXYGEN_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetOxygen : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetOxygen();
    virtual ~DatasetOxygen();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETOXYGEN_H
