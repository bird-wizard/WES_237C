/*
This is DFT computation using matrix vector multiplication form.
INPUT:
	In_R, In_I[]: Real and Imag parts of Complex signal in time domain.
OUTPUT:
	Out_R, Out_I[]: Real and Imag parts of Complex signal in frequency domain.

*/

#include<stdio.h>
#include <stdlib.h>
#include<iostream>
#include <math.h>
#include "dft.h"

struct Rmse
{
	int num_sq;
	float sum_sq;
	float error;

	Rmse(){ num_sq = 0; sum_sq = 0; error = 0; }

	float add_value(float d_n)
	{
		num_sq++;
		sum_sq += (d_n*d_n);
		error = sqrtf(sum_sq / num_sq);
		return error;
	}

};


Rmse rmse_R,  rmse_I;

float In_R[SIZE], In_I[SIZE],Out_R[SIZE],Out_I[SIZE];   //Modify the testbench while checking for demo. You will have to access the data variable of the structure//

int main()
{
	int index;
	float gold_R, gold_I;

	FILE * fp = fopen("out.gold.dat","r");

	// getting input data
	for(int i=0; i<SIZE; i++)
	{
		In_R[i] = i;
		In_I[i] = 0.0;

	}
	
    hls::stream<DTYPE> real_sample;
    hls::stream<DTYPE> imag_sample;
    hls::stream<DTYPE> real_op;
    hls::stream<DTYPE> imag_op;

	for (int i = 0; i < SIZE; i++) {
        DTYPE In_Rpkt, In_Ipkt;

        In_Rpkt.data = In_R[i];
        In_Ipkt.data = In_I[i];
		
		real_sample.write(In_Rpkt);
        imag_sample.write(In_Ipkt);
    }

	// DFT
	dft(real_sample, imag_sample, real_op, imag_op);

    for (int i = 0; i < SIZE; i++) {
        DTYPE Out_Rpkt = real_op.read();
        DTYPE Out_Ipkt = imag_op.read();

        Out_R[i] = Out_Rpkt.data;
        Out_I[i] = Out_Ipkt.data;

    }


	// comparing with golden output
	for(int i=0; i<SIZE; i++)
	{
		fscanf(fp, "%d %f %f", &index, &gold_R, &gold_I);
		rmse_R.add_value((float)Out_R[i] - gold_R);
		rmse_I.add_value((float)Out_I[i] - gold_I);
	}
	fclose(fp);


	// printing error results
	printf("----------------------------------------------\n");
	printf("   RMSE(R)           RMSE(I)\n");
	printf("%0.15f %0.15f\n", rmse_R.error, rmse_I.error);
	printf("----------------------------------------------\n");

	if (rmse_R.error > 0.1 || rmse_I.error > 0.1 ) {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "FAIL: Output DOES NOT match the golden output\n");
		fprintf(stdout, "*******************************************\n");
	    return 1;
	}else {
		fprintf(stdout, "*******************************************\n");
		fprintf(stdout, "PASS: The output matches the golden output!\n");
		fprintf(stdout, "*******************************************\n");
	    return 0;
	}

}
