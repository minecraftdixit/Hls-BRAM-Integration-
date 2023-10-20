#include <hls_stream.h>
#include "ap_fixed.h"
#include "ap_int.h"

// Define the data type for weights
typedef ap_fixed<32, 8> WEIGHT_TYPE; // Define ap_fixed with 32 bits total (including 8 fractional bits)

// Define the size of the weight arrays
const int WEIGHT_ARRAY_SIZE = 270;

// Define the array_write function with m_axi pragma
void array_write(WEIGHT_TYPE A[WEIGHT_ARRAY_SIZE], WEIGHT_TYPE B[WEIGHT_ARRAY_SIZE], ap_uint<32> *ctrl_A, ap_uint<32> *ctrl_B, ap_uint<1> start_A, ap_uint<1> start_B) {


    if (start_A) {
        for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
#pragma HLS PIPELINE
            A[i] = static_cast<WEIGHT_TYPE>(i + 1);
        }
        // Optionally, you can signal the completion to the control bus for A here.
        *ctrl_A = 1;
    }

    if (start_B) {
        for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
#pragma HLS PIPELINE
            B[i] = static_cast<WEIGHT_TYPE>(i + 1);
        }
        // Optionally, you can signal the completion to the control bus for B here.
        *ctrl_B = 1;
    }
}

// Top-level HLS function
void top_function(ap_uint<1> start_A, ap_uint<1> start_B) {

    WEIGHT_TYPE A[WEIGHT_ARRAY_SIZE];
    WEIGHT_TYPE B[WEIGHT_ARRAY_SIZE];

    // Control signals
    ap_uint<32> ctrl_A = 0;
    ap_uint<32> ctrl_B = 0;

    array_write(A, B, &ctrl_A, &ctrl_B, start_A, start_B);
}

////////////////////////////////////////////////////tb
#include <iostream>
#include <fstream>
#include "ap_fixed.h"

typedef ap_fixed<32, 8> WEIGHT_TYPE; // Define ap_fixed with 32 bits total (including 16 fractional bits)
const int WEIGHT_ARRAY_SIZE = 3366;

void array_write(WEIGHT_TYPE A[WEIGHT_ARRAY_SIZE], WEIGHT_TYPE B[WEIGHT_ARRAY_SIZE], ap_uint<32> *ctrl_A, ap_uint<32> *ctrl_B, ap_uint<1> start_A, ap_uint<1> start_B);

int main() {
    WEIGHT_TYPE A[WEIGHT_ARRAY_SIZE];
    WEIGHT_TYPE B[WEIGHT_ARRAY_SIZE];
    // Call the array_write function to initialize both arrays A and B
      ap_uint<32> ctrl_A = 0;
      ap_uint<32> ctrl_B = 0;
      ap_uint<1> start_A = 1;
      ap_uint<1> start_B = 1;

      array_write(A, B, &ctrl_A, &ctrl_B, start_A, start_B);


    // Verify the results if needed
    for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
        if (A[i] != static_cast<WEIGHT_TYPE>(i + 1) || B[i] != static_cast<WEIGHT_TYPE>(i + 1)) {
            std::cerr << "Mismatch in arrays A or B at index " << i << std::endl;
        }
    }

    // Open an input file stream to read additional weights for A from a file
    std::ifstream inputFileA("weights_A.txt");

    if (!inputFileA.is_open()) {
        std::cerr << "Error: Failed to open the input file for weights_A." << std::endl;
        return 1;
    }

    int indexA = 0;
    double weightA;
    while (inputFileA >> weightA) {
        if (indexA < WEIGHT_ARRAY_SIZE) {
            A[indexA] = static_cast<WEIGHT_TYPE>(weightA);
            indexA++;
        } else {
            std::cerr << "Warning: Input file for weights_A contains more data than the array size." << std::endl;
            break;
        }
    }

    // Close the input file stream for A
    inputFileA.close();

    // Open another input file stream to read additional weights for B from a file
    std::ifstream inputFileB("weights_B.txt");

    if (!inputFileB.is_open()) {
        std::cerr << "Error: Failed to open the input file for weights_B." << std::endl;
        return 1;
    }

    int indexB = 0;
    double weightB;
    while (inputFileB >> weightB) {
        if (indexB < WEIGHT_ARRAY_SIZE) {
            B[indexB] = static_cast<WEIGHT_TYPE>(weightB);
            indexB++;
        } else {
            std::cerr << "Warning: Input file for weights_B contains more data than the array size." << std::endl;
            break;
        }
    }

    // Close the input file stream for B
    inputFileB.close();

    // Print the weights stored in arrays A and B
    for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
    	std::cout << "A///////////////////////////////////"   << std::endl;
        std::cout << "A[" << i << "] = " << A[i] << std::endl;
        std::cout << "B[" << i << "] = " << B[i] << std::endl;
    }

    return 0;
}
