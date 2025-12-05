#include<math.h>
#include "dft.h"
#include"coefficients32_2D.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE])
{
	int i,j;
	DTYPE w;
	DTYPE c, s;

	DTYPE temp_real[SIZE];
	DTYPE temp_imag[SIZE];

	for(i=0; i<SIZE; i++){
		temp_real[i] = 0;
		temp_imag[i] = 0;
	
		w = (2.0 * 3.141592653589 / SIZE) * (DTYPE) i;

		for(j=0; j<SIZE; j++){
			c = cos_coeff_table[i][j];
			s = sin_coeff_table[i][j];

			temp_real[i] += real_sample[j] * c - imag_sample[j] * s;
			temp_imag[i] += real_sample[j] * s + imag_sample[j] * c;
		}
	}

	for(i=0; i<SIZE; i++){
		real_sample[i] = temp_real[i];
		imag_sample[i] = temp_imag[i];
	}
}
