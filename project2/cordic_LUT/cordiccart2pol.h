#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#include <ap_fixed.h>

#define DEBUG 0
#define SYNTHESIS // uncomment me for synthesis; comment me for simulation

// Question 4a: 
// How does the input data type affect the size of the LUT?
// Input data type ap_fixed<W,I> increases LUT size exponentially

// How does the output data type affect the size of the LUT?
// output data type data_t does not affect LUT size

// ap_fixed<W,I>
typedef ap_fixed<32,3> data_t; // Data type of a value in the LUT

#define W			8  // Total size of fixed-point representation
#define I			2  // Number of integer bits of fixed-point representation
#define FRACTIONAL_BITS	W-I  // Number of fractional bits
#define LUT_SIZE	(1 << (W << 1)) // Size of the LUT = 2^(2*W)

extern data_t my_LUT_r[LUT_SIZE];
extern data_t my_LUT_th[LUT_SIZE];

void cordiccart2pol(data_t x, data_t y, data_t * r,  data_t * theta);
void init_cart2pol_LUTs(data_t my_LUT_th[LUT_SIZE], data_t my_LUT_r[LUT_SIZE]);

#endif // CORDICCART2POL_H
