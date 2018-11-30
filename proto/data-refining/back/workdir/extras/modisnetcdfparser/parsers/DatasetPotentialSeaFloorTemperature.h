#ifndef DATASETPOTENTIALSEAFLOORTEMPERATURE_H
#define DATASETPOTENTIALSEAFLOORTEMPERATURE_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetPotentialSeaFloorTemperature : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetPotentialSeaFloorTemperature();
    virtual ~DatasetPotentialSeaFloorTemperature();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETPOTENTIALSEAFLOORTEMPERATURE_H
