#ifndef OGRREADER_H
#define OGRREADER_H

#include <vector>
#include <iostream>
#include <list>

#include <ogrsf_frmts.h>

#include "VectorLayerShapeType.h"
#include "ComponentNT.h"
#include "stringsplit.h"

#ifndef LOG4CPLUS_ERROR
#define LOG4CPLUS_ERROR(L,M) std::cout << "ERROR: " << M << std::endl;
#endif

#ifndef LOG4CPLUS_INFO
#define LOG4CPLUS_INFO(L,M) std::cout << "INFO: " << M << std::endl;
#endif

static std::vector<VectorLayerShapeType> geom2shapes(OGRGeometry *const geom)
{
    std::vector<VectorLayerShapeType> shapes;
    VectorLayerShapeType shape;

    //wkbPoint = 1,           /**< 0-dimensional geometric object, standard WKB */
    //wkbLineString = 2,      /**< 1-dimensional geometric object with linear
    //                         *   interpolation between Points, standard WKB */
    //wkbPolygon = 3,         /**< planar 2-dimensional geometric object defined
    //                         *   by 1 exterior boundary and 0 or more interior
    //                         *   boundaries, standard WKB */
    //wkbMultiPoint = 4,      /**< GeometryCollection of Points, standard WKB */
    //wkbMultiLineString = 5, /**< GeometryCollection of LineStrings, standard WKB */
    //wkbMultiPolygon = 6,    /**< GeometryCollection of Polygons, standard WKB */
    //wkbGeometryCollection = 7

    if (wkbFlatten(geom->getGeometryType()) == wkbPoint )
    {
        OGRPoint *pnt = (OGRPoint *)geom;
        shape.points.clear();
        shape.min.x = pnt->getX();
        shape.min.y = pnt->getY();
        shape.max = shape.min;
        shape.geomType = VectorLayerShapeType::GeometryType::Point;
        shape.shapeKind = VectorLayerShapeType::GeoShapeKind::OTHER;
        shape.coordType = VectorLayerShapeType::CoordinateType::PLANAR;
        if (geom->getSpatialReference())
            shape.coordType = VectorLayerShapeType::CoordinateType::GEO;;
        shapes.push_back(shape);
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbPolygon)
    {
        OGRPolygon *polygon = (OGRPolygon *)geom;

        int int_rings = polygon->getNumInteriorRings();

        for (int r = -1; r < int_rings; ++r)
        {
            OGRLinearRing *ring = r >= 0 ? polygon->getInteriorRing(r) : polygon->getExteriorRing();
            OGRPoint pnt;

            const int num_points = ring->getNumPoints();
            shape.points.resize(num_points);
            shape.min.x = DBL_MAX;
            shape.min.y = DBL_MAX;
            shape.max.x = -DBL_MAX;
            shape.max.y = -DBL_MAX;
            for (int p = 0; p < num_points; ++p)
            {
                ring->getPoint(p, &pnt);
                const double x = pnt.getX();
                const double y = pnt.getY();
                shape.points[p] = component2d{x, y};
                shape.min.x = std::min(shape.points[p].x, shape.min.x);
                shape.min.y = std::min(shape.points[p].y, shape.min.y);
                shape.max.x = std::max(shape.points[p].x, shape.max.x);
                shape.max.y = std::max(shape.points[p].y, shape.max.y);
            }
            shape.geomType = VectorLayerShapeType::GeometryType::Polygon;
            shape.shapeKind = VectorLayerShapeType::GeoShapeKind::OTHER;
            shape.coordType = VectorLayerShapeType::CoordinateType::PLANAR;
            if (geom->getSpatialReference())
                shape.coordType = VectorLayerShapeType::CoordinateType::GEO;;
            shapes.push_back(shape);
        }
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbMultiPoint )
    {
        OGRMultiPoint *mpnt = (OGRMultiPoint *)geom;
        const int ngeom = mpnt->getNumGeometries();
        std::vector<VectorLayerShapeType> addshapes;
        for (int i = 0; i < ngeom; ++i)
        {
            addshapes = geom2shapes(mpnt->getGeometryRef(i));
            shapes.reserve(shapes.size() + addshapes.size());
            shapes.insert(shapes.end(), addshapes.begin(), addshapes.end());
        }
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbLineString )
    {
        OGRLineString *line = (OGRLineString *)geom;
        OGRPoint pnt;
        const int num_points = line->getNumPoints();
        shape.points.resize(num_points);
        shape.min.x = DBL_MAX;
        shape.min.y = DBL_MAX;
        shape.max.x = -DBL_MAX;
        shape.max.y = -DBL_MAX;
        for (int p = 0; p < num_points; ++p)
        {
            line->getPoint(p, &pnt);
            const double x = pnt.getX();
            const double y = pnt.getY();
            shape.points[p] = component2d{x, y};
            shape.min.x = std::min(shape.points[p].x, shape.min.x);
            shape.min.y = std::min(shape.points[p].y, shape.min.y);
            shape.max.x = std::max(shape.points[p].x, shape.max.x);
            shape.max.y = std::max(shape.points[p].y, shape.max.y);
        }
        shape.geomType = VectorLayerShapeType::GeometryType::LineString;
        shape.shapeKind = VectorLayerShapeType::GeoShapeKind::OTHER;
        shape.coordType = VectorLayerShapeType::CoordinateType::PLANAR;
        if (geom->getSpatialReference())
            shape.coordType = VectorLayerShapeType::CoordinateType::GEO;;
        shapes.push_back(shape);
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbMultiLineString )
    {
        OGRMultiLineString *mline = (OGRMultiLineString *)geom;
        const int ngeom = mline->getNumGeometries();
        std::vector<VectorLayerShapeType> addshapes;
        for (int i = 0; i < ngeom; ++i)
        {
            addshapes = geom2shapes(mline->getGeometryRef(i));
            shapes.reserve(shapes.size() + addshapes.size());
            shapes.insert(shapes.end(), addshapes.begin(), addshapes.end());
        }
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbMultiPolygon )
    {
        OGRMultiPolygon *mpolygon = (OGRMultiPolygon *)geom;
        const int ngeom = mpolygon->getNumGeometries();
        std::vector<VectorLayerShapeType> addshapes;
        for (int i = 0; i < ngeom; ++i)
        {
            addshapes = geom2shapes(mpolygon->getGeometryRef(i));
            shapes.reserve(shapes.size() + addshapes.size());
            shapes.insert(shapes.end(), addshapes.begin(), addshapes.end());
        }
    }
    else if (wkbFlatten(geom->getGeometryType()) == wkbGeometryCollection )
    {
        OGRGeometryCollection *mgeom = (OGRGeometryCollection *)geom;
        const int ngeom = mgeom->getNumGeometries();
        std::vector<VectorLayerShapeType> addshapes;
        for (int i = 0; i < ngeom; ++i)
        {
            addshapes = geom2shapes(mgeom->getGeometryRef(i));
            shapes.reserve(shapes.size() + addshapes.size());
            shapes.insert(shapes.end(), addshapes.begin(), addshapes.end());
        }
    }
    else
    {
        std::cout << "skip unknown: " << wkbFlatten(geom->getGeometryType()) << std::endl;
        shape.points.clear();
    }
    return shapes;
}

static std::vector<VectorLayerShapeType> ReadOGRDataSourceToShapesFromFile(const char *const filepath)
{
    std::vector<VectorLayerShapeType> shapes;
    GDALAllRegister();
    GDALDataset *const poDS = (GDALDataset*)GDALOpenEx( filepath, GDAL_OF_VECTOR, NULL, NULL, NULL );
    if( poDS == NULL )
    {
        LOG4CPLUS_ERROR(logger, "GDALOpenEx(  ) == NULL: " << CPLGetLastErrorMsg());
        return shapes;
    }
    shapes.reserve(poDS->GetLayerCount()*16);
    OGRSpatialReference wgs84;
    wgs84.importFromEPSG(4326);
    for (int l = 0; l < poDS->GetLayerCount(); ++l)
    {
        OGRLayer* poLayer = poDS->GetLayer(l);
        OGRFeature *poFeature;
        poLayer->ResetReading();
        LOG4CPLUS_INFO(logger, "Layer "<<(poLayer->GetName())<<" has "<<(poLayer->GetFeatureCount())<<" features");
        while( (poFeature = poLayer->GetNextFeature()) != NULL )
        {
            OGRGeometry *geom = poFeature->GetGeometryRef();
            if( geom != NULL)
            {
                geom->transformTo(&wgs84);
                std::vector<VectorLayerShapeType> geom_shapes = geom2shapes(geom);
                for (auto it = geom_shapes.begin(); it != geom_shapes.end(); ++it)
                    shapes.push_back(std::move(*it));
            }
            OGRFeature::DestroyFeature( poFeature );
        }
    }
    GDALClose(poDS);
    return shapes;
}

static std::vector<VectorLayerShapeType> ReadOGRDataSourceToShapesFromWkt(const char *const wkt)
{
    std::vector<VectorLayerShapeType> shapes;
    // first check if we have srid (from postgis SRID=XXX;WKT-itself)
    std::vector<std::string> nswkt = split(wkt, ';'); //not-so-well-known-text
    if (nswkt.empty())
    {
        std::cout << "WKT is empty" << std::endl;
        return shapes;
    }
    int epsg = 0;
    for (size_t i = 0; i < nswkt.size(); ++i)
    {
        if (0 == strncmp(nswkt[i].c_str(), "SRID=", sizeof("SRID=")-1))
        {
            epsg = atoi(nswkt[i].c_str() + sizeof("SRID=")-1);
        }
        else
        {
            // nowhere stated, that OGRGeometryFactory::createFromWkt grabs srs, and not uses just it's pointer
            // so we'll init it for each iteration again, to be safe
            // P.S looks like it grabbed,  getSpatialReference() returns another pointer and deletion is safe
            OGRSpatialReference *srs = NULL;
            if (epsg > 0)
            {
                srs = new OGRSpatialReference;
                if (OGRERR_NONE != srs->importFromEPSG(epsg))
                {
                    delete srs;
                    srs = NULL;
                }
            }
            // http://www.gdal.org/classOGRGeometryFactory.html#a9418f71bcfb1e334173882c38d89d070
            // cast because OGR_G_CreateFromWkt will move the pointer
            char* pszWkt = (char*)nswkt[i].c_str();
            OGRGeometry *geom;
            OGRErr err = OGRGeometryFactory::createFromWkt(&pszWkt, srs, &geom); // srs can be null
            if (err == OGRERR_NONE)
            {
                if (!geom->getSpatialReference())
                {
                    std::cout << "WARNING: WKT geometry spatial reference is unknown" << std::endl;
                }
                else if (4326 != geom->getSpatialReference()->GetEPSGGeogCS())
                {
                    OGRSpatialReference wgs84;
                    wgs84.importFromEPSG(4326);
                    geom->transformTo(&wgs84);
                }

                std::vector<VectorLayerShapeType> geom_shapes = geom2shapes(geom);
                for (auto it = geom_shapes.begin(); it != geom_shapes.end(); ++it)
                {
                    if (VectorLayerShapeType::GeometryType::Point != it->geomType)
                        shapes.push_back(std::move(*it));
                }

                OGRGeometryFactory::destroyGeometry(geom);
                delete srs;
            }
            else
            {
                std::cout << "failed to read geometry from WKT" << std::endl;
                // we may have encounter some more shaeps
            }
        }
    }
    return shapes;
}

static std::string getPresetValue(std::string preset)
{
    if (preset == "example")
        return "wkt://SRID=4326;POLYGON ((-40 73,-39 73,-39 74,-40 74,-40 73))";
    else if (preset == "world")
        return "wkt://SRID=4326;POLYGON ((-179 -89,179 -89,179 89,-179 89,-179 -89))";
    else if (preset == "norv_sea")
        return "wkt://SRID=4326;MULTIPOLYGON (((5.897592 59.453594, -15.743695 64.412255, 15.617894 77.781198, 27.955703 70.515584, 5.897592 59.453594)), ((145.933452 43, 151.670932 43, 151.670932 39, 145.933452 39, 145.933452 43)))";
    return "";
}

static std::vector<VectorLayerShapeType> ReadOGRDataSourceToShapes(const char *const uri)
{
    //const char uri[] = "wkt://SRID=4326;POLYGON ((-24 -18,-24 -8,-20 -8,-20 -28,-24 -28))";

    if (0==strncmp(uri, "preset://", sizeof("preset://")-1))
        return ReadOGRDataSourceToShapes(getPresetValue(uri+sizeof("preset://")-1).c_str());
    else if (0==strncmp(uri, "wkt://", sizeof("wkt://")-1))
        return ReadOGRDataSourceToShapesFromWkt(uri+sizeof("wkt://")-1);
    else if (0==strncmp(uri, "file://", sizeof("file://")))
        return ReadOGRDataSourceToShapesFromFile(uri+sizeof("file://"));
    return std::vector<VectorLayerShapeType>();
}

#endif // OGRREADER_H
