#ifndef DATASETPOTENTIALTEMPERATURE_H
#define DATASETPOTENTIALTEMPERATURE_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetPotentialTemperature : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetPotentialTemperature();
    virtual ~DatasetPotentialTemperature();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETPOTENTIALTEMPERATURE_H
