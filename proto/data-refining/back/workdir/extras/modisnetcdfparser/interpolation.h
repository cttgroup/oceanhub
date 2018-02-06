#ifndef INTERPOLATION_H
#define INTERPOLATION_H

#include <cstdint>

#include "ComponentNT.h"
//#include "functions/extra.h"

/*double interpolate_from_even_quad(const double ll, const double lr, const double rl, const double rr, // values in corners, [ij]
                                         const double d1, // step from ll(lr) to rl(rr)
                                         const double d2); // step from ll(rl) to lr(rr)*/
template<class T>
static T interpolate_from_even_quad(const T ll, const T lr, const T rl, const T rr, // values in corners, [ij]
                                         const double d1, // step from ll(lr) to rl(rr)
                                         const double d2) // step from ll(rl) to lr(rr)
{ //0 <= d1 <= 1, 0 <= d2 <= 1
    const double Sll = d1*d2;
    const T val = (rr-lr-rl+ll)*Sll + (rl - ll)*d1 + (lr - ll)*d2 + ll; // <- 4 muls, 8 adds (counting Sll)
    return val;
}

/*uint16_t interpolate_quad_by_Phrogz_and_clamp(const p2d &__restrict P1,
                                              const p2d &__restrict P2,
                                              const p2d &__restrict P3,
                                              const p2d &__restrict P4,
                                              const p2d &__restrict O,
                                              const uint16_t V1, const uint16_t V2, const uint16_t V3, const uint16_t V4);//*/

//http://stackoverflow.com/questions/23920976/bilinear-interpolation-with-non-aligned-input-points
// Phrogz's interpolation
template <class T>
static T interpolate_quad_by_Phrogz_and_clamp(const component2d&__restrict P1,
                                              const component2d&__restrict P2,
                                              const component2d&__restrict P3,
                                              const component2d&__restrict P4,
                                              const component2d&__restrict O,
                                              const T V1, const T V2, const T V3, const T V4)
{ // A-B-D-C-A polygon
    // rearrange values in most useful way (so used dx will be as high as possible)
    const bool swap = (abs1d(P2.x - P1.x)+abs1d(P4.x - P3.x)) < (abs1d(P4.x - P2.x)+abs1d(P3.x - P1.x));
    component2d A,B,C,D;
    T Av,Bv,Cv,Dv;
    if (swap)
    {
        A = P2; Av = V2;
        B = P4; Bv = V4;
        C = P1; Cv = V1;
        D = P3; Dv = V3;
    }
    else
    {
        A = P1; Av = V1;
        B = P2; Bv = V2;
        C = P3; Cv = V3;
        D = P4; Dv = V4;
    }
    const double K_AB = (O.x - A.x)/(B.x - A.x);
    const double K_CD = (O.x - C.x)/(D.x - C.x);
    const double Y2 = (B.y - A.y)*K_AB + A.y;
    const double Y1 = (D.y - C.y)*K_CD + C.y;
    const double K = (O.y - Y1)/(Y2 - Y1);
    const double O2v = Av*(1-K_AB) + Bv*K_AB;
    const double O1v = Cv*(1-K_CD) + Dv*K_CD;
    const double Ov = O1v*(1-K) + O2v*K;

    const double rounder = 0.5 - (T)0.5; // for float and double rounder will become 0.5-0.5

    T Ot = (T)(Ov + rounder);
    // out value must be inside initial (becaus of clamp in function name)
    T min = V1 < V2 ? V1 : V2;
    min = V3 < min ? V3 : min;
    min = V4 < min ? V4 : min;
    T max = V1 > V2 ? V1 : V2;
    max = V3 > max ? V3 : max;
    max = V4 > max ? V4 : max;
    Ot = Ot < min ? min : Ot;
    Ot = Ot > max ? max : Ot;
    return Ot;
}//*/

#endif // INTERPOLATION_H
