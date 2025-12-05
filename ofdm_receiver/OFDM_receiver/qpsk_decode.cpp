#include "fft.h"
#include <stdio.h>

void qpsk_decode(DTYPE R[SIZE], DTYPE I[SIZE], int D[SIZE]) {


    for (int k = 0; k < SIZE; ++k) {
        DTYPE re = R[k];
        DTYPE im = I[k];

        int sym;
        if (re >= 0 && im >= 0) {
            sym = 0;
        } else if (re < 0 && im >= 0) {
            sym = 1;
        } else if (re < 0 && im < 0) {
            sym = 3;
        } else {
            sym = 2;
        }

        D[k] = sym;
    }

}

