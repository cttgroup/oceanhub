#ifndef DATASETICECONCENTRATION_H
#define DATASETICECONCENTRATION_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetIceConcentration : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetIceConcentration();
    virtual ~DatasetIceConcentration();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETICECONCENTRATION_H
