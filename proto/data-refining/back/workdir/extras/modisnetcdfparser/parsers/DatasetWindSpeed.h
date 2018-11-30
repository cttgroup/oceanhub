#ifndef DATASETWINDSPEED_H
#define DATASETWINDSPEED_H

#include "DatasetSwath.h"

class DatasetWindSpeed : public DatasetSwath
{
public:
    DatasetWindSpeed();
    virtual ~DatasetWindSpeed();

protected:
    virtual void clear() override;
    virtual bool checkTime(const int line, std::string *const ascii = NULL) override;

    virtual void fixDimensions(std::vector<size_t> *const __restrict geodims, std::vector<size_t> *const __restrict valdims) override final;
    virtual void fixValues(std::vector<double> *const values) override final;

private:
    std::vector<double> bufferTime;
};

#endif // DATASETWINDSPEED_H
