#include <stdlib.h>

typedef struct {
    double x, y, z;
} Vec3;


double multilaterate(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess, double precision);
double multilaterate_guess(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess);
Vec3 add(Vec3 a, Vec3 b);
Vec3 subtract(Vec3 a, Vec3 b);
void accum(Vec3 *a, Vec3 b);
Vec3 divide(Vec3 a, double denom);
Vec3 scale(Vec3 a, double scale);
Vec3 normalize(Vec3 a);
double magSquared(Vec3 a);
double mag(Vec3 a);
void printVec(Vec3 a);