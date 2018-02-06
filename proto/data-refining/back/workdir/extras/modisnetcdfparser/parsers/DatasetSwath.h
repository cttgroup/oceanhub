#ifndef DATASETVALUEWITHLONLAT_H
#define DATASETVALUEWITHLONLAT_H

#include <vector>
#include <string>

#include "DatasetAbstract.h"

class DatasetSwath : public DatasetAbstract
{
public:
    DatasetSwath();
    virtual ~DatasetSwath();

    virtual bool parse(const std::string &inpath, const std::string &areaURI, const std::string &outpath) override;

    void setTimeLocation(const std::list<std::string>& loc) { locTime = loc; }
    void setTimeLocation(std::list<std::string>&& loc) { locTime = std::move(loc); }

protected:
    virtual bool checkTime(const int line, std::string *const ascii = NULL);
    virtual void fixValues(std::vector<double> *const values);
    virtual void fixDimensions(std::vector<size_t> *const __restrict geodims, std::vector<size_t> *const __restrict valdims);
    bool checkNCError(int e, const std::string &t);

    int rootid;
    std::list<std::string> locTime;
};

#endif // DATASETVALUEWITHLONLAT_H
