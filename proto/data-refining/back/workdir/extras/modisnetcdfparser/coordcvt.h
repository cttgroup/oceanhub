#ifndef COORDCVT_H
#define COORDCVT_H

#include <cmath>

#include "ComponentNT.h"

static double MetersPerLatitudeDegreeAtLatitude(const double lat)
{
    const double rlat = lat*M_PI/180.0;
    return 111132.92 - 559.82*cos(2*rlat) + 1.175*cos(4*rlat);
}

static double MetersPerLongitudeDegreeAtLatitude(const double lat)
{
    const double rlat = lat*M_PI/180.0;
    return 111412.84 * cos(rlat) - 93.5 * cos(3*rlat);
}

static double distance(component2d p1, component2d p2)
{
    constexpr double R = 6371300; // metres
    p1*=(M_PI/180.0);
    p2*=(M_PI/180.0);
    component2d dcomp = p2-p1;

    double a = std::sin(dcomp.y/2) * std::sin(dcomp.y/2) +
            std::cos(p1.y) * std::cos(p2.y) *
            std::sin(dcomp.x/2) * std::sin(dcomp.x/2);
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
    double d = R * c;
    return d;
}

#endif // COORDCVT_H
