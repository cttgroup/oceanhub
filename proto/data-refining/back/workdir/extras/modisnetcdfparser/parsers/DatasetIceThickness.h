#ifndef DATASETICETHICKNESS_H
#define DATASETICETHICKNESS_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetIceThickness : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetIceThickness();
    virtual ~DatasetIceThickness();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETICETHICKNESS_H
