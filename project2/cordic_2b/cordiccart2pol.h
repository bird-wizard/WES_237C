#ifndef CORDICCART2POL_H
#define CORDICCART2POL_H

#include <ap_fixed.h>
#define NO_ITER 20

//typedef int   coef_t;
//typedef float data_t;
//typedef float acc_t;

// Question 2a:
// ap_fixed range
// -2^(I-1) to 2^(I-1) - 2^-(W-I)
// ap_fixed<W,I>

// Range of x,y: -1,1
// integer bits = 1

// Range of r: 0 to 1.414 for x = 1, y = 1
// sqrt(1 * 1 + 1 * 1) = 1.414
// 20 rotation K = 1.646760
// max_r = 1.414 * K = 2.328
// integer bits = 3

// Range of theta: -pi to +pi
// integer bits = 3

// Range of Kvalues 2^(-i) : 1 to 0.00000190734863281250
// integer bits = 1

// Range of angles atan(2^(-i)) : 0 to 0.785398163397448
// integer bits = 1

// Question 2b:
// Use the largest integer bits from 2a.
// Use 8, 12, 16, 20, 24, and 32 total bits.
typedef ap_fixed<32,3> data_t;
typedef ap_fixed<32,3> xy_t;
typedef ap_fixed<32,3> r_t;
typedef ap_fixed<32,3> theta_t;

void cordiccart2pol(xy_t x, xy_t y, r_t * r,  theta_t * theta);

#endif
