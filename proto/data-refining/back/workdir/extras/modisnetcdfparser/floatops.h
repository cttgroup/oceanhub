#ifndef FLOATOPS_H
#define FLOATOPS_H

#include <cfloat>

static constexpr float ZEROF = 8*FLT_MIN;
static constexpr double ZEROD = 16*DBL_MIN;

static inline float abs1f(const float a) { return a < 0 ? -a : a; } // names differentiation is forced
static inline int ne(const float a, const float b) { return (abs1f(a-b)>ZEROF); }
static inline int ge(const float a, const float b) { return ((a-b)>-ZEROF); }
static inline int le(const float a, const float b) { return ((a-b)<ZEROF); }

static inline double abs1d(const double a) { return a < 0 ? -a : a; }

#endif // FLOATOPS_H
