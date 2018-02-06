#ifndef DATASETSEASURFACETEMPERATURE41111_H
#define DATASETSEASURFACETEMPERATURE41111_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetSeaSurfaceTemperature41111 : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetSeaSurfaceTemperature41111();
    virtual ~DatasetSeaSurfaceTemperature41111();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETSEASURFACETEMPERATURE41111_H
