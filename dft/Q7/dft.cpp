#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

void dft(DTYPE real_sample[SIZE], DTYPE imag_sample[SIZE],DTYPE real_op[SIZE], DTYPE imag_op[SIZE])
{
	int i,j;
	DTYPE c, s;

	real_op[i] = 0.0;
	imag_op[i] = 0.0;

	for(j=0; j<SIZE; j++){
		
	
		for(i=0; i<SIZE; i++){
			int index = (i * j) % SIZE;
			c = cos_coefficients_table[index];
			s = sin_coefficients_table[index];

			real_op[i] += real_sample[j] * c - imag_sample[j] * s;
			imag_op[i] += real_sample[j] * s + imag_sample[j] * c;
		}
	}
}
