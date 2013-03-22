#include "multilateration.h"
#include <stdio.h>
#include <math.h>

#define MIC2 (Vec3){1,0,0}
#define MIC3 (Vec3){0,1,0}
#define MIC4 (Vec3){0,0,1}

void iterateN(Vec3 testPoint, unsigned int n) {
    Vec3 guess = {2, 2, 2};

    for (int i=0; i<100; i++) {
        multilaterate_guess(
            3,
            (Vec3[]){
                MIC2,
                MIC3,
                MIC4,
            },
            (double[]){
                mag(subtract(testPoint, MIC2)) - mag(testPoint),
                mag(subtract(testPoint, MIC3)) - mag(testPoint),
                mag(subtract(testPoint, MIC4)) - mag(testPoint),
            },
            &guess,
            // 0.125
            1
        );
        printf("\n");
    }
}

double quickError(Vec3 testPoint, Vec3 guess) {
    return error(
        3,
        (Vec3[]){
            MIC2,
            MIC3,
            MIC4,
        },
        (double[]){
            mag(subtract(testPoint, MIC2)) - mag(testPoint),
            mag(subtract(testPoint, MIC3)) - mag(testPoint),
            mag(subtract(testPoint, MIC4)) - mag(testPoint),
        },
        guess
    );
}

typedef double (*doubleFunc)(double, double);

typedef struct {
    double xMin;
    double xMax;
    double yMin;
    double yMax;
    unsigned int xPixels;
    unsigned int yPixels;
} ImageWindow;

void topoPlot(doubleFunc func, ImageWindow w, double topoInterval, double topoOffset) {
    double dx = (w.xMax-w.xMin) / w.xPixels;
    double dy = (w.yMax-w.yMin) / w.yPixels;

    printf("P1\n%d %d\n", w.xPixels, w.yPixels);
    for (int i=0; i<w.yPixels; i++) {
        double y = w.yMin + (w.yMax-w.yMin) * (w.yPixels - i - 1) / w.yPixels;
        for (int j=0; j<w.xPixels; j++) {
            double x = w.xMin + (w.xMax-w.xMin) * j / w.xPixels;

            double bl = (*func)(x, y);
            double tl = (*func)(x, y+dy);
            double br = (*func)(x+dx, y);
            double tr = (*func)(x+dx, y+dy);

            double min = fmin(fmin(bl, br), fmin(tl, tr));
            double max = fmax(fmax(bl, br), fmax(tl, tr));

            printf ("%d ", floor((min + topoOffset) / topoInterval) != floor((max + topoOffset) / topoInterval));
        }
        printf("\n");
    }
}

void grayScalePlot(doubleFunc func, ImageWindow w, double blackValue, double whiteValue) {
    printf("P2\n%d %d\n255\n", w.xPixels, w.yPixels);
    for (int i=0; i<w.yPixels; i++) {
        double y = w.yMin + (w.yMax-w.yMin) * (w.yPixels - i - 1) / w.yPixels;
        for (int j=0; j<w.xPixels; j++) {
            double x = w.xMin + (w.xMax-w.xMin) * j / w.xPixels;

            double value = (*func)(x, y);
            double normalized = (value - blackValue) / (whiteValue - blackValue);
            double clipped = fmax(0.0, fmin(normalized, 1.0));
            printf("%d ", (int)round(255 * clipped));
        }
        printf("\n");
    }
}

void graphError2d(Vec3 testPoint) {
    static Vec3 hurpdurp;
    hurpdurp = testPoint;
    unsigned int pixels = 800;

    double hurp(double x, double y) {
        return quickError(hurpdurp, (Vec3){x, x, y});
    }

    ImageWindow window = {-8.0, 8.0, -8.0, 8.0, pixels, pixels};
    //topoPlot(&hurp, window, 0.025, -0.01);
    hurp(0.0, 0.0);
    printf("durp");
    grayScalePlot(&hurp, window, 0.0, 2.0);

    printf("hurp");
}

void graphError1d(Vec3 testPoint) {
    unsigned int pixels = 800;
    double xMax = 10.0;
    double xMin = -10.0;
    double yMax = 1.5;

    double errorValues[pixels];

    for (int j=0; j<pixels; j++) {
        double v = xMin + (xMax - xMin) * j / pixels;
        Vec3 guess = add(testPoint, scale((Vec3){1, 1, 1}, v));
        errorValues[j] = quickError(testPoint, guess);
        // printf ("# %f\n", errorValues[j]);
    }

    // yMax = errorValues[pixels-1];


    printf("P1\n%d %d\n", pixels, pixels);
    for (int i=0; i<pixels; i++) {
        for (int j=0; j<pixels; j++) {
            printf("%d ", (pixels - i - 1) * yMax / pixels < errorValues[j]);

        }
        printf("\n");
    }
}


int main(int argc, char **argv) {
    // iterateN(100);
    graphError2d((Vec3) {1, 1, 1});
    // graphError2d();
}

