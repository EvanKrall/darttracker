#include "multilateration.h"
#include <stdio.h>

int main(int argc, char **argv) {
    Vec3 testPoint = {1, 1, 1};
    Vec3 guess = {10,10,10};

    for (int i=0; i<5; i++) {
        multilaterate_guess(
            3,
            (Vec3[]){
                (Vec3){1,0,0},
                (Vec3){0,1,0},
                (Vec3){0,0,1},
            },
            (double[]){
                mag(subtract(testPoint, (Vec3){1,0,0})) - mag(testPoint),
                mag(subtract(testPoint, (Vec3){0,1,0})) - mag(testPoint),
                mag(subtract(testPoint, (Vec3){0,0,1})) - mag(testPoint),
            },
            &guess,
            0.125
        );
        printf("\n");
    }
}