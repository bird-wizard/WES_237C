#include <ap_int.h>
#include "ap_axi_sdata.h"
#include <hls_stream.h>

#define SIZE 1024 		/* SIZE OF DFT */
typedef hls::axis<float,0,0,0> DTYPE;

void dft(hls::stream<DTYPE> &real_sample, hls::stream<DTYPE> &imag_sample, hls::stream<DTYPE> &real_op, hls::stream<DTYPE> &imag_op);
// Use streaming interface for demo
