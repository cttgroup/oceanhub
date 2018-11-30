#ifndef SQUARE_H
#define SQUARE_H

#include <iostream>
#include <set>
#include <vector>

#include "VectorLayerShapeType.h"
#include "ComponentNT.h"
#include "coordcvt.h"

static double square(const std::vector<VectorLayerShapeType> & shapes,
                    const double dy)
{
    double S = 0.0;
    std::multiset<double> crss;
    for (unsigned int i = 0; i < shapes.size(); ++i)
    {
        const std::vector<component2d> &sh = shapes[i].points;

        const int shape_size = sh.size();

        double dlat = dy/MetersPerLatitudeDegreeAtLatitude(shapes[i].min.y);
        for (double lat = shapes[i].min.y; lat < shapes[i].max.y; lat+=dlat)
        {
            crss.clear();
            int crss_size = 0;
            // y in image
            // this loop collect all shape crossing along horizontal line
            for (int j = 1; j < shape_size; ++j)
            {
                const double dy1 = sh[j-1].y - lat; // in geo
                const double dy2 = sh[j].y - lat;
                if (((dy1>=0)&(dy2<0))|((dy1<0)&(dy2>=0)))
                {
                    const double x1 = sh[j-1].x; // in geo
                    const double x2 = sh[j].x;
                    const double xmax = x1 > x2 ? x1 : x2;
                    const double xmin = x1 < x2 ? x1 : x2;
                    double x0 = (x2*dy1 - x1*dy2)/(dy1-dy2); // in geo
                    if ((x0 > xmax)|(x0 < xmin))
                        x0 = (x1+x2)/2;
                    crss.insert(x0);
                    ++crss_size;
                }
            }
            // this just extends loop with last part of shape (from last point to first)
            {
                const double dy1 = sh[shape_size-1].y - lat; // in geo
                const double dy2 = sh[0].y - lat;
                if (((dy1>=0)&(dy2<0))|((dy1<0)&(dy2>=0)))
                {
                    const double x1 = sh[shape_size-1].x; // in geo
                    const double x2 = sh[0].x;
                    const double xmax = x1 > x2 ? x1 : x2;
                    const double xmin = x1 < x2 ? x1 : x2;
                    double x0 = (x2*dy1 - x1*dy2)/(dy1-dy2); // in geo
                    if ((x0 > xmax)|(x0 < xmin))
                        x0 = (x1+x2)/2;
                    crss.insert(x0);
                    ++crss_size;
                }
            }
            if (crss_size%2) // if we somehow entered closed circuit and never get out...
            { // shall never enter this branch
                std::cout << "shape " << i << " at lat = " << lat << " has " << crss.size() << " crossings" << std::endl;
                continue;
            }

            if (!crss_size)
                continue; // no hit
            std::vector<double> crssv; crssv.reserve(crss_size);
            for (auto it = crss.cbegin(); it != crss.cend(); ++it)
                crssv.push_back(*it);

            dlat = dy / MetersPerLatitudeDegreeAtLatitude(lat);
            const double coef = MetersPerLongitudeDegreeAtLatitude(lat)*dy;

            for (size_t j = 0; j < crss.size(); j+=2)
                S += (crssv[j+1]-crssv[j])*coef;
        }
    }
    return S;
}

#endif // SQUARE_H
