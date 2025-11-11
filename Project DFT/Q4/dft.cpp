#include<math.h>
#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE], DTYPE * outReal, DTYPE * outImag)
{
	int i,j;
	DTYPE w;
	DTYPE c, s;

	for(i=0; i<SIZE; i++){
		outReal[i] = 0;
		outImag[i] = 0;
	
		w = (2.0 * 3.141592653589 / SIZE) * (DTYPE) i;

		#pragma HLS PIPELINE II=1
		for(j=0; j<SIZE; j++){
			c = cos_coeff_table[i][j];
			s = sin_coeff_table[i][j];

			outReal[i] += real_sample[j] * c - imag_sample[j] * s;
			outImag[i] += real_sample[j] * s + imag_sample[j] * c;
		}
	}
}
