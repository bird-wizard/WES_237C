#include<math.h>
#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE outReal[SIZE], DTYPE outImag[SIZE])
{
	#pragma HLS ARRAY_PARTITION variable=cos_coeff_table block dim=2 factor=32
	#pragma HLS ARRAY_PARTITION variable=sin_coeff_table block dim=2 factor=32
	#pragma HLS ARRAY_PARTITION variable=real_sample block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=imag_sample block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=outReal block dim=1 factor=32
	#pragma HLS ARRAY_PARTITION variable=outImag block dim=1 factor=32
	
	
	int i,j;
	DTYPE w;
	DTYPE c, s;

	for(i=0; i<SIZE; i++){
		w = (2.0 * 3.141592653589 / SIZE) * (DTYPE) i;
		#pragma HLS UNROLL factor=32
		for(j=0; j<SIZE; j++){
			c = cos_coeff_table[i][j];
			s = sin_coeff_table[i][j];

			outReal[i] += real_sample[j] * c - imag_sample[j] * s;
			outImag[i] += real_sample[j] * s + imag_sample[j] * c;
		}
	}
}
