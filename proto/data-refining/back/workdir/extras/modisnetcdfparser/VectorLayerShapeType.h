#ifndef SHAPETYPE_H
#define SHAPETYPE_H

#include <vector>

#include "ComponentNT.h"

struct VectorLayerShapeType {
    enum class GeoShapeKind {
        GSHHS_L1,
        GSHHS_L2,
        OTHER
    };
    enum class CoordinateType {
        GEO,
        PLANAR
    };
    enum class GeometryType {
        LineString,
        Polygon,
        Point,
        Other
    };
    std::string name;
    std::vector<component2d> points;
    component2d min;
    component2d max;
    GeoShapeKind shapeKind;
    CoordinateType coordType;
    GeometryType geomType;
    int level;
};

#endif // SHAPETYPE_H

