#ifndef DATASETTIMEDEPTHLATLON_H
#define DATASETTIMEDEPTHLATLON_H

#include <cstdint>

#include <string>
#include <list>
#include <vector>

#include "DatasetAbstract.h"

// for reading dataset with folowing specifics:
// 1. Time,Depth,Latitude,Longitude are one-dimesional
// 2. Value dataset itself is 4-dimensional* with corresonding dimensions order (time,depth,latitude,longitude)
//    * if empty location is set for any of (time,depth), it's dimension is skipped

class DatasetValue4Time1Depth1Lat1Lon1 : public DatasetAbstract
{
public:
    DatasetValue4Time1Depth1Lat1Lon1();
    virtual ~DatasetValue4Time1Depth1Lat1Lon1();
    virtual bool parse(const std::string &inpath, const std::string& areaURI, const std::string& outpath) override;

    void setTimeLocation(const std::list<std::string>& loc) { locTime = loc; }
    void setTimeLocation(std::list<std::string>&& loc) { locTime = std::move(loc); }
    void setDepthLocation(const std::list<std::string>& loc) { locDepth = loc; }
    void setDepthLocation(std::list<std::string>&& loc) { locDepth = std::move(loc); }

protected:
    bool checkNCError(int e, const std::string &t);
    virtual void cvtFileTimeToHrsSince1900(std::vector<int64_t> *const timedata) const;

    std::list<std::string> locTime;
    std::list<std::string> locDepth;
    int rootid;
};

#endif // DATASETTIMEDEPTHLATLON_H
