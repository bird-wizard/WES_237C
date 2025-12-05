#include "fft.h"

static unsigned short count;
static DTYPE xr[ SIZE ];
static DTYPE xi[ SIZE ];
static DTYPE xr_out[ SIZE ];
static DTYPE xi_out[ SIZE ];
static int   dout[ SIZE ];

void ofdm_receiver( 
	hls::stream<axis_t> &inptr, 
	hls::stream<axis_t> &outptr 
){
#pragma HLS INTERFACE mode=axis port=inptr
#pragma HLS INTERFACE mode=axis port=outptr
#pragma HLS INTERFACE mode=s_axilite port=return

	for (int i = 0; i < SIZE; i++) {
		#pragma HLS PIPELINE II=1
		axis_t in_r = inptr.read();
		axis_t in_i = inptr.read();

		xr[i] = in_r.data;
		xi[i] = in_i.data;
	}
	demod( xr, xi, dout, xr_out, xi_out );
	
	for(int i = 0; i < SIZE; i++){
		#pragma HLS PIPELINE II=1
		axis_t out_d;
		out_d.data = dout[i];
		if (i == SIZE - 1) {
			out_d.last = 1;
			out_d.keep = 0xF;
			out_d.strb = 0xF;
		} else {
			out_d.keep = 0xF;
			out_d.strb = 0xF;
			out_d.last = 0;
		}
		outptr.write(out_d);
	}
}
