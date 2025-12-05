#include<math.h>
#include "dft.h"
#include"coefficients1024.h"

void dft(hls::stream<DTYPE> &real_sample, hls::stream<DTYPE> &imag_sample, hls::stream<DTYPE> &real_op, hls::stream<DTYPE> &imag_op)
{

	#pragma HLS INTERFACE mode=axis port=real_sample, imag_sample, real_op, imag_op
	#pragma HLS INTERFACE mode=s_axilite port=return
	//fp_int Adata, Bdata, Cdata, Ddata;
	DTYPE real_samplepkt, imag_samplepkt, real_oppkt, imag_oppkt;
	float real_sampledata[SIZE], imag_sampledata[SIZE],real_opdata[SIZE], imag_opdata[SIZE];

	int i,j;
	float c, s;
	for(i=0; i<SIZE; i++){
		real_samplepkt = real_sample.read();
		imag_samplepkt = imag_sample.read();
		real_sampledata[i] = real_samplepkt.data;
		imag_sampledata[i] = imag_samplepkt.data;
	}
	for(i=0; i<SIZE; i++){
		real_opdata[i] = 0.0;
		imag_opdata[i] = 0.0;

		for(j=0; j<SIZE; j++){
			int index = (i * j) % SIZE;
			c = cos_coefficients_table[index];
			s = sin_coefficients_table[index];

			real_opdata[i] += real_sampledata[j] * c - imag_sampledata[j] * s;
			imag_opdata[i] += real_sampledata[j] * s + imag_sampledata[j] * c;
		}
	}
	for(i=0; i<SIZE; i++){
		real_oppkt.data = real_opdata[i];
		imag_oppkt.data = imag_opdata[i];

		if (i == SIZE - 1) {
			real_oppkt.last = 1;
			imag_oppkt.last = 1;
		} else {
			real_oppkt.last = 0;
			imag_oppkt.last = 0;
		}
		
		real_op.write(real_oppkt);
		imag_op.write(imag_oppkt);
	}
}
