#include "multilateration.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

double multilaterate(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess, double alpha, double precision) {
    // We assume that there is a receiver at {0,0,0} with deltaDistance of 0 - that is, it's the receiver to which all the distance deltas are referenced.
    // So if there are 8 receivers, numKnownPoints should be 7.
    
    double errorSquared;
    do {
        errorSquared = multilaterate_guess(numKnownPoints, knownPoints, deltaDistances, guess, alpha);
    } while (errorSquared > precision * precision);
    return errorSquared;
}

double multilaterate_guess(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess, double alpha) {
    double distanceToOrigin = mag(*guess);
    Vec3 gradient = {0, 0, 0};

    printVec("guess", *guess);
    for (int i=0; i < numKnownPoints; i++) {
        Vec3 p = knownPoints[i];
        printVec("\tp", p);

        Vec3 vectorToP = subtract(p, *guess);
        printVec("\tvectorToP", vectorToP);
        double targetDistance = distanceToOrigin + deltaDistances[i];
        printf("\ttargetDistance %f\n", targetDistance);
        Vec3 targetVector = scale(vectorToP, targetDistance / mag(vectorToP));
        printVec("\ttargetVector", targetVector);

        Vec3 force = add(vectorToP, targetVector);
        printVec("\tforce", force);
        accum(&gradient, divide(force, numKnownPoints)); // Check direction here.
        printf("\n");
    }

    printVec("gradient", gradient);

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

void printVec(char *name, Vec3 a) {
    printf("%s: % -3.3f % -3.3f % -3.3f\n", name, a.x, a.y, a.z);
}