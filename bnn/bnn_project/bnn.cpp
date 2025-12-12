#include <stdint.h>
#include "bnn.h"
#include "golden.h"

static inline int16_t XNOR(int16_t a, int16_t b) {
    return (a == b) ? 1 : 0;
}

static inline int16_t dot_xnor(const DTYPE *A, const int *B, int N) {
    int16_t cnt = 0;
    for (int i = 0; i < N; i++){
        cnt += XNOR(A[i], (int16_t) B[i]);
    }
    int16_t ret = (int16_t)(2 * cnt - N);
    return ret;
}

void bnn(DTYPE IN[784], ITYPE ys[10])
{
    #pragma HLS INTERFACE s_axilite port=return bundle=CTRL

    #pragma HLS INTERFACE s_axilite port=IN   bundle=CTRL
    #pragma HLS INTERFACE s_axilite port=ys   bundle=CTRL

    #pragma HLS INTERFACE m_axi     port=IN offset=slave bundle=DATA
    #pragma HLS INTERFACE m_axi     port=ys offset=slave bundle=DATA

    #pragma HLS DATAFLOW

    int16_t layer1[128];

    for (int j = 0; j < 128; j++) {
        layer1[j] = dot_xnor(IN, fc1w[j], 784);
    }

    int16_t layer2[64];
    int16_t l1_sign[128];

    for (int i = 0; i < 128; i++)
        l1_sign[i] = (layer1[i] > 0 ? 1 : -1);

    for (int j = 0; j < 64; j++)
        layer2[j] = dot_xnor(l1_sign, fc2w[j], 128);


    int16_t l2_sign[64];

    for (int i = 0; i < 64; i++)
        l2_sign[i] = (layer2[i] > 0 ? 1 : -1);

    for (int j = 0; j < 10; j++) {
        ys[j] = dot_xnor(l2_sign, fc3w[j], 64);
    }
}
