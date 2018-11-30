#ifndef DATASETCARBON_H
#define DATASETCARBON_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetCarbon : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetCarbon();
    virtual ~DatasetCarbon();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETCARBON_H
