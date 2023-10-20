#include <hls_stream.h>
#include "ap_fixed.h"

typedef ap_fixed<32, 8> WEIGHT_TYPE;

const int WEIGHT_ARRAY_SIZE = 1480;

void array_write(WEIGHT_TYPE conv1_weight_q0_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE shuffle_conv_3x3_q0_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE shuffle_conv_3x3_q1_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE shuffle_conv_1x1_q0_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE shuffle_conv_1x1_q1_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE conv_last_weight_q0_0[WEIGHT_ARRAY_SIZE],
                 WEIGHT_TYPE fc_weight_q0_0[WEIGHT_ARRAY_SIZE],
                 ap_uint<32> *ctrl,
                 ap_uint<1> start) {
#pragma HLS INTERFACE ap_none port=conv1_weight_q0_0
#pragma HLS INTERFACE ap_none port=shuffle_conv_3x3_q0_0
#pragma HLS INTERFACE ap_none port=shuffle_conv_3x3_q1_0
#pragma HLS INTERFACE ap_none port=shuffle_conv_1x1_q0_0
#pragma HLS INTERFACE ap_none port=shuffle_conv_1x1_q1_0
#pragma HLS INTERFACE ap_none port=conv_last_weight_q0_0
#pragma HLS INTERFACE ap_none port=fc_weight_q0_0
#pragma HLS INTERFACE ap_none port=ctrl
#pragma HLS INTERFACE ap_ctrl_none port=return

    if (start) {
        for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
#pragma HLS PIPELINE off

            conv1_weight_q0_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            shuffle_conv_3x3_q0_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            shuffle_conv_3x3_q1_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            shuffle_conv_1x1_q0_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            shuffle_conv_1x1_q1_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            conv_last_weight_q0_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
            fc_weight_q0_0[i] = static_cast<WEIGHT_TYPE>(i + 1);
        }
        *ctrl = 1;
    }
}
