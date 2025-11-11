#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
	#pragma HLS ARRAY_PARTITION variable=cos_coefficients_table block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=sin_coefficients_table block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=real_sample block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=imag_sample block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=real_op block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=imag_op block dim=1 factor=32
	
	int i,j;
	DTYPE c, s;
	for(i=0; i<SIZE; i++){
		real_op[i] = 0.0;
		imag_op[i] = 0.0;
	}
	
	for(j=0; j<SIZE; j++){
		for(i=0; i<SIZE; i++){
			#pragma HLS PIPELINE II=16
			#pragma HLS UNROLL factor=16
			int index = (i * j) % SIZE;
			c = cos_coefficients_table[index];
			s = sin_coefficients_table[index];

			real_op[i] += real_sample[j] * c - imag_sample[j] * s;
			imag_op[i] += real_sample[j] * s + imag_sample[j] * c;
		}
	}
}
