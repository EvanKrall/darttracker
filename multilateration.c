#include "multilateration.h"
#include <stdlib.h>
#include <math.h>

double multilaterate(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess, double precision) {
    // We assume that there is a receiver at {0,0,0} with deltaDistance of 0 - that is, it's the receiver to which all the distance deltas are referenced.
    // So if there are 8 receivers, numKnownPoints should be 7.
    
    double errorSquared;
    do {
        errorSquared = multilaterate_guess(numKnownPoints, knownPoints, deltaDistances, guess);
    } while (errorSquared > precision * precision);
    return errorSquared;
}

double multilaterate_guess(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess) {
    double distanceToOrigin = mag(*guess);
    Vec3 gradient = {0, 0, 0};
    double alpha = 1.0;

    for (int i=0; i < numKnownPoints; i++) {
        Vec3 p = knownPoints[i];

        Vec3 vectorToP = subtract(p, *guess);
        double targetDistance = distanceToOrigin + deltaDistances[i];
        Vec3 targetVector = scale(vectorToP, targetDistance / mag(vectorToP));
        accum(&gradient, subtract(vectorToP, targetVector)); // Check direction here.
    }

    accum(guess, scale(gradient, alpha));
    return mag(gradient);
}

Vec3 add(Vec3 a, Vec3 b) {
    return (Vec3){
        a.x+b.x,
        a.y+b.y,
        a.z+b.z,
    };
}

Vec3 subtract(Vec3 a, Vec3 b) {
    return (Vec3) {
        a.x-b.x,
        a.y-b.y,
        a.z-b.z,
    };
}

void accum(Vec3 *a, Vec3 b) {
    a->x += b.x;
    a->y += b.y;
    a->z += b.z;
}

Vec3 divide(Vec3 a, double denom) {
    return (Vec3){
        a.x / denom,
        a.y / denom,
        a.z / denom,
    };
}

Vec3 scale(Vec3 a, double scale) {
    return (Vec3){
        a.x * scale,
        a.y * scale,
        a.z * scale,
    };
}

Vec3 normalize(Vec3 a) {
    return divide(a, mag(a));
}

double magSquared(Vec3 a) {
    return (a.x) * (a.x) + (a.y) * (a.y) + (a.z) * (a.z);
}

double mag(Vec3 a) {
    return sqrt(magSquared(a));
}

void printVec(Vec3 a) {
    printf("% -3.3f % -3.3f % -3.3f\n", a.x, a.y, a.z);
}