/*
 This is traditional 2-radix DIT FFT algorithm implementation.
 INPUT:
 In_R, In_I[]: Real and Imag parts of Complex signal

 OUTPUT:
 Out_R, Out_I[]: Real and Imag parts of Complex signal
 */

#include "fft.h"

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE X_R_Copy[SIZE], DTYPE X_I_Copy[SIZE]);
void fft_stage_first(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int STAGES, DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);
void fft_stage_last(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]);
void qpsk_decode(DTYPE R[SIZE], DTYPE I[SIZE], int D[SIZE]);

void demod(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int D[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{
	fft(X_R, X_I, OUT_R, OUT_I);
	qpsk_decode(OUT_R, OUT_I, D);
}

void fft(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE])
{
	#pragma HLS DATAFLOW
	
	//Call fft
	DTYPE BT_R[SIZE], BT_I[SIZE], OUT_I_Copy[SIZE], OUT_R_Copy[SIZE];
	DTYPE group1_R[SIZE], group1_I[SIZE];
	DTYPE group2_R[SIZE], group2_I[SIZE];

	// bit reverse input
	bit_reverse(X_R, X_I, BT_R, BT_I);

	// first group of fft 1-3
	fft_group1(BT_R, BT_I, group1_R, group1_I);
	
	// second group of fft 4-6
	fft_group2(group1_R, group1_I, group2_R, group2_I);
	
	// third group of fft 7-10
	fft_group3(group2_R, group2_I, OUT_R_Copy, OUT_I_Copy);

	// bit reverse output
	bit_reverse(OUT_R_Copy, OUT_I_Copy, OUT_R, OUT_I);
}

void fft_group1(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	DTYPE temp1_R[SIZE], temp1_I[SIZE];
	DTYPE temp2_R[SIZE], temp2_I[SIZE];
	fft_stages(    X_R,     X_I, 1, temp1_R, temp1_I);
	fft_stages(temp1_R, temp1_I, 2, temp2_R, temp2_I);
	fft_stages(temp2_R, temp2_I, 3,   OUT_R,   OUT_I);
}

void fft_group2(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	DTYPE temp1_R[SIZE], temp1_I[SIZE];
	DTYPE temp2_R[SIZE], temp2_I[SIZE];
	fft_stages(    X_R,     X_I, 4, temp1_R, temp1_I);
	fft_stages(temp1_R, temp1_I, 5, temp2_R, temp2_I);
	fft_stages(temp2_R, temp2_I, 6,   OUT_R,   OUT_I);
}

void fft_group3(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	DTYPE temp1_R[SIZE], temp1_I[SIZE];
	DTYPE temp2_R[SIZE], temp2_I[SIZE];
	fft_stages(    X_R,     X_I, 7, temp1_R, temp1_I);
	fft_stages(temp1_R, temp1_I, 8, temp2_R, temp2_I);
	fft_stages(temp2_R, temp2_I, 9,   temp1_R,  temp1_I);
	fft_stages(temp1_R, temp1_I, 10,   OUT_R,   OUT_I);
}

void bit_reverse(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	#pragma HLS INLINE off
	#pragma HLS ARRAY_PARTITION variable=OUT_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_I cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=X_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=X_I cyclic factor=4
	
	int input,rev;
	for (int i = 0; i < SIZE; i++) {
		rev = 0;
		input = i;
		for (int j = 0; j < M; j++) {
			rev = (rev << 1) | (input & 1);
			input = input >> 1;
		}
		OUT_R[rev] = X_R[i];
		OUT_I[rev] = X_I[i];
	}


}
/*=======================BEGIN: FFT=========================*/
//stage 1
void fft_stage_first(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	#pragma HLS INLINE off
	#pragma HLS ARRAY_PARTITION variable=X_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=X_I cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_I cyclic factor=4

	fft_stages(X_R, X_I, 1, OUT_R, OUT_I);
}

//stages
void fft_stages(DTYPE X_R[SIZE], DTYPE X_I[SIZE], int stage, DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	#pragma HLS INLINE off
	#pragma HLS ARRAY_PARTITION variable=X_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=X_I cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_I cyclic factor=4

	int DFTpts = 1 << stage;
    int numBF  = DFTpts >> 1;  
    int step   = SIZE >> stage; 

    int j, i;

    for (j = 0; j < numBF; j++) {
		#pragma HLS LOOP UNROLL factor=4

	    int k = j * step;
        DTYPE c = W_real[k];
        DTYPE s = W_imag[k];

        for (i = j; i < SIZE; i += DFTpts) {
			#pragma HLS PIPELINE II=1
	       	int i_lower = i + numBF;

            DTYPE temp_R = X_R[i_lower] * c - X_I[i_lower] * s;
            DTYPE temp_I = X_I[i_lower] * c + X_R[i_lower] * s;

            OUT_R[i_lower] = X_R[i] - temp_R;
            OUT_I[i_lower] = X_I[i] - temp_I;
            OUT_R[i]       = X_R[i] + temp_R;
            OUT_I[i]       = X_I[i] + temp_I;
        }
    }

}


//last stage
void fft_stage_last(DTYPE X_R[SIZE], DTYPE X_I[SIZE], DTYPE OUT_R[SIZE], DTYPE OUT_I[SIZE]) {
	#pragma HLS INLINE off
	#pragma HLS ARRAY_PARTITION variable=X_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=X_I cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_R cyclic factor=4
	#pragma HLS ARRAY_PARTITION variable=OUT_I cyclic factor=4

	fft_stages(X_R, X_I, M, OUT_R, OUT_I);

}
/*=======================END: FFT=========================*/
