#ifndef DATASETIRON_H
#define DATASETIRON_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetIron : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetIron();
    virtual ~DatasetIron();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETIRON_H
