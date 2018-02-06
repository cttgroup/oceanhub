#ifndef DATASETABSTRACT_H
#define DATASETABSTRACT_H

#include <string>
#include <list>

class DatasetAbstract
{
public:
    DatasetAbstract();
    virtual ~DatasetAbstract();
    virtual bool parse(const std::string &inpath, const std::string& areaURI, const std::string& outpath) = 0;

    void setLatitudeLocation(const std::list<std::string>& loc) { locLatitude = loc; }
    void setLatitudeLocation(std::list<std::string>&& loc) { locLatitude = std::move(loc); }
    void setLongitudeLocation(const std::list<std::string>& loc) { locLongitude = loc; }
    void setLongitudeLocation(std::list<std::string>&& loc) { locLongitude = std::move(loc); }
    void setValueLocation(const std::list<std::string>& loc) { locValue = loc; }
    void setValueLocation(std::list<std::string>&& loc) { locValue = std::move(loc); }

    bool setUtcTimeSince(const std::string& ascii);
    bool setUtcTimeTill(const std::string& ascii);
    void setResolution(const double res);

protected:
    virtual void clear();

    std::list<std::string> locLatitude;
    std::list<std::string> locLongitude;
    std::list<std::string> locValue;
    int64_t since1900H;
    int64_t till1900H;
    double resolution; // meters
    std::string outbuffer;
};

#endif // DATASETABSTRACT_H
