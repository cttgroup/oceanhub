#ifndef ATTRIBUTES_H
#define ATTRIBUTES_H

#ifdef __GNUC__
// do they work when put before function name?
#define ATTR_CONST __attribute__((const))
#define ATTR_PURE __attribute__((pure))
#define RESTRICT __restrict
#else
#define ATTR_CONST
#define ATTR_PURE
#define RESTRICT
#endif

#endif // ATTRIBUTES_H
