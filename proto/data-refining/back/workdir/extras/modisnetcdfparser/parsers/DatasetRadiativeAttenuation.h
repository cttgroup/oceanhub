#ifndef DATASETRADIATIVEATTENUATION_H
#define DATASETRADIATIVEATTENUATION_H

#include <cstdint>
#include <vector>

#include "DatasetValue4Time1Depth1Lat1Lon1.h"

class DatasetRadiativeAttenuation : public DatasetValue4Time1Depth1Lat1Lon1
{
public:
    DatasetRadiativeAttenuation();
    virtual ~DatasetRadiativeAttenuation();

protected:
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const override;
};

#endif // DATASETRADIATIVEATTENUATION_H
