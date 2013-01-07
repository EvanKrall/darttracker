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


// double stddev(size_t n, double *v) {
//     double sumSquares = 0;
//     double mean = avg(n, v);
//     for (int i=0; i<n; i++) {
//         sumSquares += pow(v[i] - mean);
//     }
//     return sqrt(sumSquares / n);
// }

// double avg(size_t n, double *v) {
//     double sum = 0;
//     for (int i=0; i<n; i++) {
//         sum += v[i];
//     }
//     return sum;
// }

double error(size_t numKnownPoints, Vec3 *knownPoints, double *targetDeltaDistances, Vec3 point) {
    // Return approximately how wrong point is.
    // double *deltaDistanceErrors = calloc(sizeof(double), numKnownPoints);
    double distanceToOrigin = mag(point);

    double sum = 0;
    for (int i=0; i<numKnownPoints; i++) {
        double actualDeltaDistance = mag(subtract(point, knownPoints[i])) - distanceToOrigin;
        sum += pow(actualDeltaDistance - targetDeltaDistances[i], 2);
    }
    return sqrt(sum / numKnownPoints);
}

double multilaterate_guess(size_t numKnownPoints, Vec3 *knownPoints, double *deltaDistances, Vec3 *guess, double alpha) {
    // double distanceToOrigin = mag(*guess);
    Vec3 gradient = {0, 0, 0};

    printVec("guess", *guess);
    double currentError = error(numKnownPoints, knownPoints, deltaDistances, *guess);

    double dd = 0.0001;
    printf("currentError %f\n", currentError);

    gradient = divide((Vec3) {
        error(numKnownPoints, knownPoints, deltaDistances, add(*guess, (Vec3){dd, 0, 0})) - currentError,
        error(numKnownPoints, knownPoints, deltaDistances, add(*guess, (Vec3){0, dd, 0})) - currentError,
        error(numKnownPoints, knownPoints, deltaDistances, add(*guess, (Vec3){0, 0, dd})) - currentError,
    }, dd);

    printVec("gradient", gradient);

    accum(guess, scale(gradient, -1.0 * alpha));
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
    // printf("%s: % -3.3f % -3.3f % -3.3f\n", name, a.x, a.y, a.z);
    printf ("%s %e %e %e\n", name, a.x, a.y, a.z);
}