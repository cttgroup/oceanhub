#ifndef DATASETPHYTOPLANKTONCARBON_H
#define DATASETPHYTOPLANKTONCARBON_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetPhytoplanktonCarbon : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetPhytoplanktonCarbon();
    virtual ~DatasetPhytoplanktonCarbon();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETPHYTOPLANKTONCARBON_H
