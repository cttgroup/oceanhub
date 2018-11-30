#ifndef DATASETCHLOROPHYLL41111_H
#define DATASETCHLOROPHYLL41111_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetChlorophyll41111 : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetChlorophyll41111();
    virtual ~DatasetChlorophyll41111();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;

};

#endif // DATASETCHLOROPHYLL41111_H
