#ifndef COMPONENTNT_H
#define COMPONENTNT_H

#include <iostream>

#include "floatops.h"
#include "attributes.h"

#ifdef PAD_DATA
#define VECTOR3_SIZE 4
#else
#define VECTOR3_SIZE 3
#endif

struct component3f;
struct component3d;
struct component3i32;

/////////////////////////////////////////////////////
/// \brief The component3f struct ///////////////////
/////////////////////////////////////////////////////
struct component3f {
    union {
        struct {
            float x,y,z;
        };
        float cols[VECTOR3_SIZE];
    };

    const float& operator[](const int i) const { return cols[i]; }
    float& operator[](const int i) { return cols[i]; }
    explicit operator component3d() const;
    explicit operator component3f() const;

    // these will deny using constexpr:
    //component3f() : x{float()}, y{float()}, z{float()} { return; }
    //component3f(const component3f& p) : x{p.x}, y{p.x}, z{p.x} { return; }
    //component3f(const float X, const float Y, const float Z) : x{X}, y{Y}, z{Z} { return; }
    //component3f(const float scalar) : x{scalar}, y{scalar}, z{scalar} { return; }
    //component3f& operator=(const float scalar) { *this = component3f{scalar, scalar, scalar}; return *this; }
};

////////////////////////////////////////////////////////
/// \brief The component3f operators ///////////////////
////////////////////////////////////////////////////////
static inline component3f operator-(const component3f& a, const component3f& b) { return component3f{a.x - b.x, a.y - b.y, a.z - b.z}; }
static inline component3f operator-(const component3f& a, const float b) { return component3f{a.x - b, a.y - b, a.z - b}; }
static inline component3f operator+(const component3f& a, const component3f& b) { return component3f{a.x + b.x, a.y + b.y, a.z + b.z}; }
static inline component3f operator+(const component3f& a, const float b) { return component3f{a.x + b, a.y + b, a.z + b}; }
static inline component3f operator*(const component3f& a, const float b) { return component3f{a.x * b, a.y * b, a.z * b}; }
static inline component3f operator*(const component3f& a, const component3f& b) { return component3f{a.x * b.x, a.y * b.y, a.z * b.z}; }
static inline component3f operator/(const component3f& a, const component3f& b) { return component3f{a.x / b.x, a.y / b.y, a.z / b.z}; }
static inline component3f operator/(const component3f& a, const float b) { return component3f{a.x / b, a.y / b, a.z / b}; }
static inline component3f operator/(const float a, const component3f& b) { return component3f{a / b.x, a / b.y, a / b.z}; }
static inline component3f& operator*=(component3f& a, const float b) { a.x *= b; a.y *= b; a.z *= b; return a; }
static inline component3f& operator+=(component3f& a, const float b) { a.x += b; a.y += b; a.z += b; return a; }
static inline component3f& operator+=(component3f& a, const component3f& b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
static inline component3f& operator*=(component3f& a, const component3f& b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return a; }
static inline component3f absof(const component3f &RESTRICT a) {
    return component3f{ a.x < 0 ? -a.x : a.x,
                        a.y < 0 ? -a.y : a.y,
                        a.z < 0 ? -a.z : a.z };
}
static inline component3f minof(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3f{ a.x < b.x ? a.x : b.x,
                        a.y < b.y ? a.y : b.y,
                        a.z < b.z ? a.z : b.z};
}
static inline component3f maxof(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3f{ a.x > b.x ? a.x : b.x,
                        a.y > b.y ? a.y : b.y,
                        a.z > b.z ? a.z : b.z };

}
///////////////////////////////////////////////////////
/// \brief The component3i32 struct ///////////////////
///////////////////////////////////////////////////////
struct component3i32 {
    union {
        struct {
            int32_t x,y,z;
        };
        int32_t cols[VECTOR3_SIZE];
    };
};

//////////////////////////////////////////////////////////
/// \brief The component3i32 functions ///////////////////
//////////////////////////////////////////////////////////
static inline component3i32 operator|(const component3i32 &RESTRICT a, const component3i32 &RESTRICT b) {
    return component3i32{(a.x|b.x),(a.y|b.y),(a.z|b.z)};
}
static inline component3i32& operator|=(component3i32 &RESTRICT a, const component3i32 &RESTRICT b) {
    a.x|=b.x; a.y|=b.y; a.z|=b.z; return a;
}
static inline component3i32 operator&(const component3i32 &RESTRICT a, const component3i32 &RESTRICT b) {
    return component3i32{(a.x&b.x),(a.y&b.y),(a.z&b.z)};
}
static inline int32_t h_or(const component3i32 &RESTRICT a) {
    return -(a.x|a.y|a.z); // 0xFFFFFFFF -> 0x00000001
}
static inline int32_t h_and(const component3i32 &RESTRICT a) {
    return -(a.x&a.y&a.z);
}

//////////////////////////////////////////////////////////////////////
/// \brief The component3i32/component3f functions ///////////////////
//////////////////////////////////////////////////////////////////////

static inline component3i32 gr(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x > b.x), -(int32_t)(a.y > b.y), -(int32_t)(a.z > b.z)};
}
static inline component3i32 grz(const component3f &RESTRICT a) {
    return component3i32{-(int32_t)(a.x > 0), -(int32_t)(a.y > 0), -(int32_t)(a.z > 0)};
}
static inline component3i32 ge(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x >= b.x), -(int32_t)(a.y >= b.y), -(int32_t)(a.z >= b.z)};
}
static inline component3i32 gee(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x > (b.x - ZEROF)), -(int32_t)(a.y > (b.y - ZEROF)), -(int32_t)(a.z > (b.z - ZEROF))};
}
static inline component3i32 ls(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x < b.x), -(int32_t)(a.y < b.y), -(int32_t)(a.z < b.z)};
}
static inline component3i32 lsz(const component3f &RESTRICT a) {
    return component3i32{-(int32_t)(a.x < 0), -(int32_t)(a.y < 0), -(int32_t)(a.z < 0)};
}
static inline component3i32 le(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x <= b.x), -(int32_t)(a.y <= b.y), -(int32_t)(a.z <= b.z)};
}
static inline component3i32 lee(const component3f &RESTRICT a, const component3f &RESTRICT b) {
    return component3i32{-(int32_t)(a.x < (b.x + ZEROF)), -(int32_t)(a.y < (b.y + ZEROF)), -(int32_t)(a.z < (b.z + ZEROF))};
}



struct component3d {
    union {
        struct {
            double x,y,z;
        };
        double cols[VECTOR3_SIZE];
    };

    const double& operator[](const int i) const { return cols[i]; }
    double& operator[](const int i) { return cols[i]; }
    explicit operator component3d() const;
    explicit operator component3f() const;

};
////////////////////////////////////////////////////////
/// \brief The component3d operators ///////////////////
////////////////////////////////////////////////////////
static inline component3d operator-(const component3d& a, const component3d& b) { return component3d{a.x - b.x, a.y - b.y, a.z - b.z}; }
static inline component3d operator-(const component3d& a, const double b) { return component3d{a.x - b, a.y - b, a.z - b}; }
static inline component3d operator+(const component3d& a, const component3d& b) { return component3d{a.x + b.x, a.y + b.y, a.z + b.z}; }
static inline component3d operator+(const component3d& a, const float b) { return component3d{a.x + b, a.y + b, a.z + b}; }
static inline component3d operator*(const component3d& a, const double b) { return component3d{a.x * b, a.y * b, a.z * b}; }
static inline component3d operator*(const component3d& a, const component3d& b) { return component3d{a.x * b.x, a.y * b.y, a.z * b.z}; }
static inline component3d operator/(const component3d& a, const component3d& b) { return component3d{a.x / b.x, a.y / b.y, a.z / b.z}; }
static inline component3d operator/(const component3d& a, const double b) { return component3d{a.x / b, a.y / b, a.z / b}; }
static inline component3d operator/(const double a, const component3d& b) { return component3d{a / b.x, a / b.y, a / b.z}; }
static inline component3d& operator*=(component3d& a, const double b) { a.x *= b; a.y *= b; a.z *= b; return a; }
static inline component3d& operator+=(component3d& a, const double b) { a.x += b; a.y += b; a.z += b; return a; }
static inline component3d& operator+=(component3d& a, const component3d& b) { a.x += b.x; a.y += b.y; a.z += b.z; return a; }
static inline component3d& operator-=(component3d& a, const component3d& b) { a.x -= b.x; a.y -= b.y; a.z -= b.z; return a; }
static inline component3d& operator*=(component3d& a, const component3d& b) { a.x *= b.x; a.y *= b.y; a.z *= b.z; return a; }
static inline component3d absof(const component3d &RESTRICT a) {
    return component3d{ a.x < 0 ? -a.x : a.x,
                        a.y < 0 ? -a.y : a.y,
                        a.z < 0 ? -a.z : a.z };
}
static inline component3d minof(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3d{ a.x < b.x ? a.x : b.x,
                        a.y < b.y ? a.y : b.y,
                        a.z < b.z ? a.z : b.z};
}
static inline component3d maxof(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3d{ a.x > b.x ? a.x : b.x,
                        a.y > b.y ? a.y : b.y,
                        a.z > b.z ? a.z : b.z };

}

///////////////////////////////////////////////////////
/// \brief The component3i32 struct ///////////////////
///////////////////////////////////////////////////////
struct component3i64 {
    union {
        struct {
            int64_t x,y,z;
        };
        int64_t cols[VECTOR3_SIZE];
    };
};

//////////////////////////////////////////////////////////
/// \brief The component3i64 functions ///////////////////
//////////////////////////////////////////////////////////
static inline component3i64 operator|(const component3i64 &RESTRICT a, const component3i64 &RESTRICT b) {
    return component3i64{(a.x|b.x),(a.y|b.y),(a.z|b.z)};
}
static inline component3i64& operator|=(component3i64 &RESTRICT a, const component3i64 &RESTRICT b) {
    a.x|=b.x; a.y|=b.y; a.z|=b.z; return a;
}
static inline component3i64 operator&(const component3i64 &RESTRICT a, const component3i64 &RESTRICT b) {
    return component3i64{(a.x&b.x),(a.y&b.y),(a.z&b.z)};
}
static inline int64_t h_or(const component3i64 &RESTRICT a) {
    return -(a.x|a.y|a.z); // 0xFFFFFFFF -> 0x00000001
}
static inline int64_t h_and(const component3i64 &RESTRICT a) {
    return -(a.x&a.y&a.z);
}

//////////////////////////////////////////////////////////////////////
/// \brief The component3i64/component3d functions ///////////////////
//////////////////////////////////////////////////////////////////////

static inline component3i64 gt(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x > b.x), -(int64_t)(a.y > b.y), -(int64_t)(a.z > b.z)};
}
static inline component3i64 gtz(const component3d &RESTRICT a) {
    return component3i64{-(int64_t)(a.x > 0), -(int64_t)(a.y > 0), -(int64_t)(a.z > 0)};
}
static inline component3i64 ge(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x >= b.x), -(int64_t)(a.y >= b.y), -(int64_t)(a.z >= b.z)};
}
static inline component3i64 gee(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x > (b.x - ZEROD)), -(int64_t)(a.y > (b.y - ZEROD)), -(int64_t)(a.z > (b.z - ZEROD))};
}
static inline component3i64 lt(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x < b.x), -(int64_t)(a.y < b.y), -(int64_t)(a.z < b.z)};
}
static inline component3i64 ltz(const component3d &RESTRICT a) {
    return component3i64{-(int64_t)(a.x < 0), -(int64_t)(a.y < 0), -(int64_t)(a.z < 0)};
}
static inline component3i64 le(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x <= b.x), -(int64_t)(a.y <= b.y), -(int64_t)(a.z <= b.z)};
}
static inline component3i64 lee(const component3d &RESTRICT a, const component3d &RESTRICT b) {
    return component3i64{-(int64_t)(a.x < (b.x + ZEROD)), -(int64_t)(a.y < (b.y + ZEROD)), -(int64_t)(a.z < (b.z + ZEROD))};
}

struct component2f {
    union {
        struct {
            float x,y;
        };
        float cols[2];
    };

    const float& operator[](const int i) const { return cols[i]; }
    float& operator[](const int i) { return cols[i]; }

    // these will deny using constexpr:
    //vec3f() : x{float()}, y{float()}, z{float()} { return; }
    //vec3f(const vec3f& p) : x{p.x}, y{p.x}, z{p.x} { return; }
    //vec3f(const float X, const float Y, const float Z) : x{X}, y{Y}, z{Z} { return; }
    //vec3f(const float scalar) : x{scalar}, y{scalar}, z{scalar} { return; }
    //vec3f& operator=(const float scalar) { *this = vec3f{scalar, scalar, scalar}; return *this; }
};
static inline component2f operator-(const component2f& a, const component2f& b) { return component2f{a.x - b.x, a.y - b.y }; }
static inline component2f operator+(const component2f& a, const component2f& b) { return component2f{a.x + b.x, a.y + b.y }; }
static inline component2f operator*(const component2f& a, const float b) { return component2f{a.x * b, a.y * b }; }
static inline component2f& operator*=(component2f& a, const float b) { a.x *= b; a.y *= b; return a; }
static inline component2f& operator+=(component2f& a, const component2f& b) { a.x += b.x; a.y += b.y; return a; }
static inline component2f& operator*=(component2f& a, const component2f& b) { a.x *= b.x; a.y *= b.y; return a; }

struct component2d {
    union {
        struct {
            double x,y;
        };
        double cols[2];
    };

    const double& operator[](const int i) const { return cols[i]; }
    double& operator[](const int i) { return cols[i]; }
};
static inline component2d operator-(const component2d& a, const component2d& b) { return component2d{a.x - b.x, a.y - b.y }; }
static inline component2d operator+(const component2d& a, const component2d& b) { return component2d{a.x + b.x, a.y + b.y }; }
static inline component2d operator*(const component2d& a, const double b) { return component2d{a.x * b, a.y * b }; }
static inline component2d& operator*=(component2d& a, const double b) { a.x *= b; a.y *= b; return a; }
static inline component2d& operator+=(component2d& a, const component2d& b) { a.x += b.x; a.y += b.y; return a; }
static inline component2d& operator*=(component2d& a, const component2d& b) { a.x *= b.x; a.y *= b.y; return a; }

inline component3f::operator component3d() const { return component3d{(double)x,(double)y,(double)z}; }
inline component3f::operator component3f() const { return *this; }
inline component3d::operator component3f() const { return component3f{(float)x,(float)y,(float)z}; }
inline component3d::operator component3d() const { return *this; }
static inline component2d absof(const component2d &RESTRICT a) {
    return component2d{ a.x < 0 ? -a.x : a.x,
                        a.y < 0 ? -a.y : a.y };
}
static inline component2d minof(const component2d &RESTRICT a, const component2d &RESTRICT b) {
    return component2d{ a.x < b.x ? a.x : b.x,
                        a.y < b.y ? a.y : b.y };
}
static inline component2d maxof(const component2d &RESTRICT a, const component2d &RESTRICT b) {
    return component2d{ a.x > b.x ? a.x : b.x,
                        a.y > b.y ? a.y : b.y };
}

struct component2i32 {
    union {
        struct {
            int32_t x,y;
        };
        int32_t cols[2];
    };

    const int32_t& operator[](const int i) const { return cols[i]; }
    int32_t& operator[](const int i) { return cols[i]; }
};
static inline component2i32 operator-(const component2i32& a, const component2i32& b) { return component2i32{a.x - b.x, a.y - b.y }; }
static inline component2i32 operator+(const component2i32& a, const component2i32& b) { return component2i32{a.x + b.x, a.y + b.y }; }
static inline component2i32 operator*(const component2i32& a, const int32_t b) { return component2i32{a.x * b, a.y * b }; }
static inline component2i32& operator*=(component2i32& a, const int32_t b) { a.x *= b; a.y *= b; return a; }
static inline component2i32& operator+=(component2i32& a, const component2i32& b) { a.x += b.x; a.y += b.y; return a; }
static inline component2i32& operator*=(component2i32& a, const component2i32& b) { a.x *= b.x; a.y *= b.y; return a; }


static_assert(sizeof(component3f)==(VECTOR3_SIZE*sizeof(float)), "vec3f size mismatch");
static_assert(sizeof(component3d)==(VECTOR3_SIZE*sizeof(double)), "vec3d size mismatch");

static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component2f &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y << "} ";
    return os;
}
static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component3f &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y << ", " << v.z << "} ";
    return os;
}
static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component2d &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y<< "} ";
    return os;
}
static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component3d &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y << ", " << v.z << "} ";
    return os;
}
static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component2i32 &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y << "} ";
    return os;
}
static inline std::ostream& operator<<(std::ostream &RESTRICT os, const component3i32 &RESTRICT v)
{
    os << "{" << v.x << ", " << v.y << ", " << v.z << "} ";
    return os;
}



#endif // COMPONENTNT_H
