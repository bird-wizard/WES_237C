/*
	Filename: fir.cpp
		Complex FIR or Match filter
		firI1 and firI2 share coef_t c[N]
		firQ1 and firQ2 share coef_t c[N]
		
	INPUT:
		I: signal for I sample
		I: signal for Q sample

	OUTPUT:
		X: filtered output
		Y: filtered output

*/

#include "phasedetector.h"


void firI1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	static
		data_t shift_reg[N];
		acc_t acc;
		int i;
	//#pragma HLS array_partition variable=c complete
	//#pragma HLS array_partition variable=shift_reg complete
	
	acc = 0;
	//#pragma HLS pipeline off
	TDL:
	for (i = N - 1; i > 0; i--) {
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		//#pragma HLS unroll factor=127
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MAC:
	for (i = N - 1; i >= 0; i--){
		//#pragma HLS unroll factor=64
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		acc += shift_reg[i] * c[i];
	}
	
	*y = acc;
}


void firI2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {1,    -1,    1,    -1,    -1,    -1,    1,    1,    -1,    -1,    -1,    1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    1,    1,    1,    1,    -1,    -1,    1,    1,    1,    -1,    -1,    -1};

	// Write your code here
	static
		data_t shift_reg[N];
		acc_t acc;
		int i;
	//#pragma HLS array_partition variable=c complete
	//#pragma HLS array_partition variable=shift_reg complete
	
	acc = 0;
	//#pragma HLS pipeline off
	TDL:
	for (i = N - 1; i > 0; i--) {
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		//#pragma HLS unroll factor=127
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MAC:
	for (i = N - 1; i >= 0; i--){
		//#pragma HLS unroll factor=64
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		acc += shift_reg[i] * c[i];
	}
	
	*y = acc;
}




void firQ1 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static
		data_t shift_reg[N];
		acc_t acc;
		int i;
	//#pragma HLS array_partition variable=c complete
	//#pragma HLS array_partition variable=shift_reg complete
	
	acc = 0;
	//#pragma HLS pipeline off
	TDL:
	for (i = N - 1; i > 0; i--) {
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		//#pragma HLS unroll factor=127
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MAC:
	for (i = N - 1; i >= 0; i--){
		//#pragma HLS unroll factor=64
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		acc += shift_reg[i] * c[i];
	}
	
	*y = acc;
}

void firQ2 (
  data_t *y,
  data_t x
  ) {

	coef_t c[N] = {-1,    -1,    1,    -1,    1,    -1,    1,    -1,    -1,    -1,    -1,    1,    -1,    1,    -1,    1,    1,    -1,    1,    -1,    -1,    1,    -1,    1,    1,    1,    1,    -1,    1,    -1,    1,    1};


	// Write your code here
	static
		data_t shift_reg[N];
		acc_t acc;
		int i;
	//#pragma HLS array_partition variable=c complete
	//#pragma HLS array_partition variable=shift_reg complete
	
	acc = 0;
	//#pragma HLS pipeline off
	TDL:
	for (i = N - 1; i > 0; i--) {
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		//#pragma HLS unroll factor=127
		shift_reg[i] = shift_reg[i - 1];
	}
	shift_reg[0] = x;

	MAC:
	for (i = N - 1; i >= 0; i--){
		//#pragma HLS unroll factor=64
		//#pragma HLS pipeline off
		//#pragma HLS pipeline II=1
		acc += shift_reg[i] * c[i];
	}
	
	*y = acc;	
}


void fir (
  data_t I,
  data_t Q,

  data_t *X,
  data_t *Y
  ) {

	// Write your code here
	
	//Calculate X
	//Calculate Y
	data_t IinIfir,QinQfir,QinIfir,IinQfir;

	firI1(&IinIfir, I);
	firQ1(&QinQfir, Q);
	firI2(&QinIfir, Q);
	firQ2(&IinQfir, I);
	
	*X = IinIfir + QinQfir;
	*Y = QinIfir - IinQfir;

}
