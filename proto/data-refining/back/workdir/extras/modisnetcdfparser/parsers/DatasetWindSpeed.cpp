#include "DatasetWindSpeed.h"

#include <limits>

#include <netcdf.h>

#include "readVariable.h"

DatasetWindSpeed::DatasetWindSpeed() : DatasetSwath()
{
    locValue.push_back("Geophysical Data");
    locLongitude.push_back("Longitude of Observation Point");
    locLatitude.push_back("Latitude of Observation Point");
}

DatasetWindSpeed::~DatasetWindSpeed()
{

}

void DatasetWindSpeed::fixValues(std::vector<double> * const values)
{
    // although never described in attributes wind has a lot of -32768 values
    // that looks like NO_DATA, also they are only negative values
    // and since negative wind speed is illogical, we'll mark by it (it's also scaled at this moment to something like -32.7679999)
    for (size_t i = 0; i < values->size(); ++i)
        (*values)[i] = (*values)[i] < 0 ? std::numeric_limits<double>::quiet_NaN() : (*values)[i];
    return;
}

void DatasetWindSpeed::fixDimensions(std::vector<size_t> * const geodims, std::vector<size_t> * const valdims)
{
    // special case for extra last dimension being 1
    if ((valdims->size() == (geodims->size()+1))&&(valdims->back()==1)) // also assures that valdims has at least 1 element
    {
        valdims->pop_back();
    }
    return;
}

bool DatasetWindSpeed::checkTime(const int line, std::string * const ascii)
{
    // don't know what time is in file, will return false only on file access errors

    if (locTime.empty())
    {
        if (ascii)
            *ascii = "undefined";
        return true;
    }
    std::vector<char> units;

    double scale = 1.0;
    if (bufferTime.empty() & (rootid >= 0))
    {
        size_t totallen = 1;
        std::vector<char> txt;
        txt.assign(locTime.back().size()+1,0);
        memcpy(txt.data(), locTime.back().c_str(), locTime.back().size());
        int varid;
        int status = nc_inq_varid(rootid, txt.data(), &varid);
        if (status == NC_NOERR)
        {
            int ndims;
            status = nc_inq_varndims(rootid, varid, &ndims);
            if ((status == NC_NOERR)&(ndims==1))
            {
                std::vector<int> dimids;
                dimids.resize(ndims);
                status = nc_inq_vardimid(rootid, varid, dimids.data());
                if (status == NC_NOERR)
                {
                    size_t inclen = 0;
                    for (int i = 0; (i < ndims); ++i)
                    {
                        status = nc_inq_dimlen(rootid, dimids[i], &inclen);
                        totallen *= inclen;
                        if (status != NC_NOERR)
                            break;
                    }
                }
            }
        }
        if (status != NC_NOERR)
        {
            LOG("Failure=Failed to get time data");
            if (ascii)
                *ascii = "undefined";
            return false;
        }
        bufferTime.resize(totallen);
        status = nc_get_var_double(rootid, varid, bufferTime.data());

        status = nc_get_att_double(rootid, varid, "SCALE FACTOR", &scale);
        if (status!=NC_NOERR)
        {
            for (size_t i = 0; i < bufferTime.size(); ++i)
                bufferTime[i] *= scale;
        }

        size_t len;
        status = nc_inq_attlen(rootid, varid, "UNIT", &len);
        if (status==NC_NOERR)
        {
            units.assign(len+1,0);
            status = nc_get_att_text(rootid, varid, "UNIT", units.data());
            if (status!=NC_NOERR)
            {
                units.front() = 0;
            }
        }
    }

    if (ascii)
    {
        if ((line < 0)|(line >= bufferTime.size()))
        {
            if (ascii)
                *ascii = "undefined";
            return false;
        }

        *ascii = std::to_string(bufferTime[line]);
        if (!units.empty())
            *ascii += units.data();
    }
    return true;
}

void DatasetWindSpeed::clear()
{
    bufferTime.clear();
    return;
}
