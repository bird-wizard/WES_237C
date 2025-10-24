#include "cordiccart2pol.h"
#include <cmath>

// Pre-computed values for CORDIC shifts
// Kvalues[i] = 2^(-i)
data_t Kvalues[NO_ITER] = {
	1,	
	0.500000000000000,	
	0.250000000000000,	
	0.125000000000000,	
	0.0625000000000000,	
	0.0312500000000000,	
	0.0156250000000000,	
	0.00781250000000000,	
	0.00390625000000000,	
	0.00195312500000000,	
	0.000976562500000000,	
	0.000488281250000000,	
	0.000244140625000000,	
	0.000122070312500000,	
	6.10351562500000e-05,	
	3.05175781250000e-05,
	0.0000152587890625000,
	0.00000762939453125000,
	0.00000381469726562500,
	0.00000190734863281250
};

// angles[i] = atan(2^(-i))
theta_t angles[NO_ITER] = {
	0.785398163397448,	
	0.463647609000806,	
	0.244978663126864,	
	0.124354994546761,	
	0.0624188099959574,	
	0.0312398334302683,	
	0.0156237286204768,	
	0.00781234106010111,	
	0.00390623013196697,	
	0.00195312251647882,	
	0.000976562189559320,	
	0.000488281211194898,	
	0.000244140620149362,	
	0.000122070311893670,	
	6.10351561742088e-05,	
	3.05175781155261e-05,
	0.0000152587890613158,
	0.00000762939453110197,
	0.00000381469726560650,
	0.00000190734863281019
};


// Rotate the vector (x,y) to the x-axis
// r = sqrt(x^2 + y^2)
// theta = atan2(y,x)
// k = product of 1/sqrt(1+Kvalues[i]^2)
// x goes to r scaled by a gain k
// y goes to 0
// theta accumulates the angle of rotation
void cordiccart2pol(xy_t x, xy_t y, r_t * r,  theta_t * theta) {
	// Major HLS optimization is choosing the correct number representation for the variables
	
	// Pre-compute scale factor K	
	// 20 rotation K = 0.646760
	data_t K = 0.646760;
	theta_t pi = 3.1415926535897932;
	//for (i = 0; i < NO_ITER; i++) {
	//	K = K * ((data_t) 1.0 / std::sqrt((data_t) 1.0 + Kvalues[i] * Kvalues[i]));
	//}

	// Initialize output angle
	*theta = (theta_t) 0.0;
	
	// The first step of the algorithm performs a rotation to get the initial vector in either Quadrant I or IV.
	// This rotates the vector by +/- 90 degrees depending on the sign of the y value of the initial vector.
	// If the y value is positive, we know that we are in either Quadrant I or II.
	xy_t x_temp;
	if (x < 0 && y == 0) {
		// On negative x-axis, rotate +180 degrees
		// accumulate angle of +180 degrees
		*theta += pi;
		*r = (r_t) 1.0;
		return;
	} else if (x > 0 && y == 0) {
		// On positive x-axis, no rotation needed
		*r = (r_t) 1.0;
		*theta = (theta_t) 0.0;
		return;
	} else if (x < 0 && y > 0) {
		// x < 0 and y > 0 = Quadrant II
		// Rotate -90 degrees

		// Recall that a +/- 90 degree rotation is done by negating either the x or y values of the vector
		// and then swapping those values.
		x_temp = -x;
		x = y;
		y = x_temp;
		
		// accumulate angle of -90 degrees
		*theta +=  pi / (theta_t) 2.0; 
	} else if (x < 0 && y < 0) {
		// x < 0 and y < 0 = Quadrant III
		// Rotate +90 degrees
		x_temp = x;
		x = -y;
		y = x_temp;

		// accumulate angle of +90 degrees
		*theta -= pi / (theta_t) 2.0;
	}

	// Should now be in Quadrant I / IV.

	xy_t x_current = x;
	xy_t y_current = y;
	xy_t x_next, y_next;
	int direction;

	// For each angle rotation
	for (int i = 0; i < NO_ITER; i++) {
		// If y is negative, rotate counter-clockwise up to zero.
		if (y_current < 0) {
			direction = 1;
			*theta -= angles[i];
		} else {
			// y is positive, rotate clockwise down to zero.
			direction = -1;
			*theta += angles[i];
		}
		// Equation 3.23 from Parallel Programming for FPGAs
		x_next = x_current - direction * y_current * Kvalues[i];
		y_next = x_current * direction * Kvalues[i] + y_current;
		
		// Next iteration
		x_current = x_next;
		y_current = y_next;
	}

	*r = x_current * K; 
}