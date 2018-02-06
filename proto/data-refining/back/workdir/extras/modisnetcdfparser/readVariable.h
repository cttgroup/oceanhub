#ifndef READVARIABLE_H
#define READVARIABLE_H

#include <cstdint>
#include <cstring>
#include <iostream>
#include <set>
#include <list>
#include <vector>
#include <limits>
#include <netcdf.h>
#include "logger.h"

#define ERR__(e,t) {LOG("Error: "<< nc_strerror(e) << "\nhint: "<< t <<"\n"<<"Result=Failure"); exit(2);}
#define CHECK(e,t) if (e!=NC_NOERR) ERR__(e,t)

template<class T>
static T noData();

template<>
double noData<double>() { return std::numeric_limits<double>::quiet_NaN(); /*(int64_t*)&NO_DATA = -1; */ }
template<>
int noData<int>() { return std::numeric_limits<int>::lowest(); }
template<>
long noData<long>() { return std::numeric_limits<long>::lowest(); }
template<>
long long noData<long long>() { return std::numeric_limits<long long>::lowest(); }

template<class T>
static int nc_get_att_typed(int ncid, int varid, const char *name, T *ip);

template<>
int nc_get_att_typed<double>(int ncid, int varid, const char *name, double *ip)
{
    return nc_get_att_double(ncid, varid, name, ip);
}

template<>
int nc_get_att_typed<int>(int ncid, int varid, const char *name, int *ip)
{
    return nc_get_att_int(ncid, varid, name, ip);
}

template<>
int nc_get_att_typed<int64_t>(int ncid, int varid, const char *name, int64_t *ip)
{
    return nc_get_att_longlong(ncid, varid, name, (long long*)ip);
}

template<class T>
static int nc_get_var_typed(int ncid, int varid, T *ip);

template<>
int nc_get_var_typed<double>(int ncid, int varid, double *ip)
{
    return nc_get_var_double(ncid, varid, ip);
}

template<>
int nc_get_var_typed<int>(int ncid, int varid, int *ip)
{
    return nc_get_var_int(ncid, varid, ip);
}

template<>
int nc_get_var_typed<int64_t>(int ncid, int varid, int64_t *ip)
{
    return nc_get_var_longlong(ncid, varid, (long long*)ip);
}

template<class T>
static int readVariable(int gid, const std::list<std::string>& path,
                        std::vector<T> *data,
                        std::vector<size_t> *dimensions)
{
    if (path.empty())
        return -1;
    int status;
    const std::string &name = path.back();

    for (auto it = path.cbegin(); it != --path.cend(); ++it)
    {
        status = nc_inq_grp_ncid(gid, (char*)it->c_str(), &gid);
        CHECK(status, *it);
    }

    int varid;
    status = nc_inq_varid(gid, (char*)name.c_str(), &varid);
    CHECK(status, name);
    int ndims;
    status = nc_inq_varndims(gid, varid, &ndims);
    CHECK(status, name);
    std::vector<int> dimids;
    dimids.resize(ndims);
    status = nc_inq_vardimid(gid, varid, dimids.data());
    CHECK(status, name);
    dimensions->resize(ndims);
    size_t totallen = 1;
    for (int i = 0; i < ndims; ++i)
    {
        status = nc_inq_dimlen(gid, dimids[i], dimensions->data() + i);
        totallen *= (*dimensions)[i];
        CHECK(status, name);
    }
    data->resize(totallen);
    nc_type data_type;
    status = nc_inq_vartype(gid, varid, &data_type);
    CHECK(status, name);

    switch (data_type)
    {
    case NC_BYTE:
    case NC_CHAR:
    case NC_SHORT:
    case NC_INT:
    case NC_FLOAT:
    case NC_DOUBLE:
    case NC_UBYTE:
    case NC_USHORT:
    case NC_UINT:
    case NC_INT64:
    case NC_UINT64: break;
    default: LOG("Unsuppported type " << data_type);
    }

    // test if data needs rescaling
    T scale = 1;
    bool need_transform = false;
    status = nc_get_att_typed<T>(gid, varid, "scale_factor", &scale);
    if (status!=NC_ENOTATT)
    {
        need_transform = true;
    }
    else
    {
        status = nc_get_att_typed<T>(gid, varid, "SCALE FACTOR", &scale);
        if (status!=NC_ENOTATT)
        {
            need_transform = true;
        }
        else
            scale = 1;
    }

    T offset;
    status = nc_get_att_typed<T>(gid, varid, "add_offset", &offset);
    if (status!=NC_ENOTATT)
    {
        need_transform = true;
    }
    else
        offset = 0;

    bool check_fillValue = false;
    nc_type fillValue_type;
    status = nc_inq_atttype(gid, varid, "_FillValue", &fillValue_type);
    if (status!=NC_ENOTATT)
    {
        if (fillValue_type == data_type)
            check_fillValue = true;
        else
            LOG("Unable to check fillValue, types of variable and attribute _fillValue do not match")
    }


    if (!check_fillValue)
    {
        status = nc_get_var_typed<T>(gid, varid, data->data());
        CHECK(status, name);
    }
    else
    {
        uint8_t fillValue[32];
        status = nc_get_att   (gid, varid, "_FillValue", &fillValue);
        CHECK(status, name);
        size_t sz;
        status = nc_inq_type(gid, fillValue_type, NULL, &sz);

        std::vector<uint8_t> buffer;
        buffer.resize(totallen*sz);

        status = nc_get_var(gid, varid, buffer.data());
        CHECK(status, name);

        for (size_t i = 0; i < totallen; ++i)
        {
            if (0 == memcmp(fillValue, buffer.data() + sz*i, sz))
                (*data)[i] = noData<T>();
            else
            {
                switch (data_type)
                {
                case NC_BYTE: (*data)[i] = (T)*(int8_t*)(buffer.data() + i*sz); break;
                case NC_CHAR: (*data)[i] = (T)*(char*)(buffer.data() + i*sz); break;
                case NC_SHORT: (*data)[i] = (T)*(int16_t*)(buffer.data() + i*sz); break;
                case NC_INT: (*data)[i] = (T)*(int32_t*)(buffer.data() + i*sz); break;
                case NC_FLOAT: (*data)[i] = (T)*(float*)(buffer.data() + i*sz); break;
                case NC_DOUBLE: (*data)[i] = (T)*(double*)(buffer.data() + i*sz); break;
                case NC_UBYTE: (*data)[i] = (T)*(uint8_t*)(buffer.data() + i*sz); break;
                case NC_USHORT: (*data)[i] = (T)*(uint16_t*)(buffer.data() + i*sz); break;
                case NC_UINT: (*data)[i] = (T)*(uint32_t*)(buffer.data() + i*sz); break;
                case NC_INT64: (*data)[i] = (T)*(int64_t*)(buffer.data() + i*sz); break;
                case NC_UINT64: (*data)[i] = (T)*(uint64_t*)(buffer.data() + i*sz); break;
                default: (*data)[i] = noData<T>();
                }
            }
        }
    }
    CHECK(status, name);

    if (need_transform)
    {
        for (size_t i = 0; i < totallen; ++i)
            (*data)[i] = (*data)[i] * scale + offset;
    }

    return NC_NOERR;
}
#undef CHECK

#endif // READVARIABLE_H
