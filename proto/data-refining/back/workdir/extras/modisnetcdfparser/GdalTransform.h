#ifndef GDALTRANSFORM_H
#define GDALTRANSFORM_H

#include <iostream>
#include <vector>
#include <string>
#include <ogrsf_frmts.h>
#include "ComponentNT.h"

static OGRSpatialReferenceH srsFromWkt(const std::string &wkt)
{
    // horrific gdal design for import from wkt
    // it will change pointer(should not destroy any data)
    // it also asks for pointer to non-const data (std::string without const cast won't do)
    if (wkt.size() > 1024)
    {
        return NULL;
    }
    char **fromSRSWkt = new char*[2];
    fromSRSWkt[0] = new char[wkt.size()+1];
    strcpy(fromSRSWkt[0], wkt.c_str()); fromSRSWkt[wkt.size()] = 0;
    fromSRSWkt[1] = NULL;
    OGRSpatialReferenceH srs = OSRNewSpatialReference(NULL);
    OGRErr err = OSRImportFromWkt(srs, fromSRSWkt);
    delete fromSRSWkt[0];
    delete fromSRSWkt;
    if (OGRERR_NONE != err)
    {
        std::cout << "Failed to create Spatial Reference fron wkt: " << wkt << std::endl;
        OSRDestroySpatialReference(srs);
        return NULL;
    }
    return srs;
}

static OGRSpatialReferenceH srsFromEpsg(const int epsg)
{
    OGRSpatialReferenceH srs = OSRNewSpatialReference(NULL);
    OGRErr err = OSRImportFromEPSG(srs, epsg);
    if (OGRERR_NONE != err)
    {
        std::cout << "Failed to create Spatial Reference fron epsg: " << epsg << std::endl;
        OSRDestroySpatialReference(srs);
        return NULL;
    }
    return srs;
}

static void srsDestroy(OGRSpatialReferenceH srs)
{
    if (srs)
        OSRDestroySpatialReference(srs);
}

static bool transformPoints(double *__restrict x,
                            double *__restrict y,
                            int count,
                            OGRSpatialReferenceH __restrict fromSRS,
                            OGRSpatialReferenceH __restrict toSRS)
{
    if (!fromSRS||!toSRS)
        return false;
    if (fromSRS==toSRS)
        return true;

    OGRCoordinateTransformationH warp = OCTNewCoordinateTransformation(fromSRS, toSRS);
    if (!warp)
    {
        std::cout << "Failed to create Coordinate Transformation" << std::endl;
        return false;
    }

    bool result = (TRUE == OCTTransform(warp, count, x, y, NULL));
    OCTDestroyCoordinateTransformation(warp);

    return result;
}

/*static bool transformPoints(std::vector<component2d> *__restrict xy,
                            OGRSpatialReferenceH __restrict fromSRS,
                            OGRSpatialReferenceH __restrict toSRS)
{
    if (!fromSRS||!toSRS)
        return false;
    if (fromSRS==toSRS)
        return true;

    OGRCoordinateTransformationH warp = OCTNewCoordinateTransformation(fromSRS, toSRS);
    if (!warp)
    {
        std::cout << "Failed to create Coordinate Transformation" << std::endl;
        return false;
    }

    std::vector<double> x,y;
    x.resize(xy->size());
    y.resize(xy->size());
    for (size_t i = 0; i < xy->size(); ++i)
    {
        x[i] = (xy->data()+i)->x;
        y[i] = (xy->data()+i)->y;
    }

    const bool result = (TRUE == OCTTransform(warp, (int)x.size(), x.data(), y.data(), NULL));
    OCTDestroyCoordinateTransformation(warp);

    if (result)
    {
        for (size_t i = 0; i < xy->size(); ++i)
        {
            (xy->data()+i)->x = x[i];
            (xy->data()+i)->y = y[i];
        }
    }
    return result;
}//*/

#endif // GDALTRANSFORM_H



































