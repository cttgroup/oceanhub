#ifndef DATASETDENSITYOCEANMIXEDLAYERTHICKNESS_H
#define DATASETDENSITYOCEANMIXEDLAYERTHICKNESS_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetDensityOceanMixedLayerThickness : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetDensityOceanMixedLayerThickness();
    virtual ~DatasetDensityOceanMixedLayerThickness();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETDENSITYOCEANMIXEDLAYERTHICKNESS_H
