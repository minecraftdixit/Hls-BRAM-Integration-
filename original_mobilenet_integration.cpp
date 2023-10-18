#include <hls_stream.h>
#include "ap_fixed.h"

// Define the data type for weights
typedef ap_fixed<32, 8> WEIGHT_TYPE; // Define ap_fixed with 32 bits total (including 8 fractional bits)

// Define the size of the weight array
const int WEIGHT_ARRAY_SIZE = 200000;

// Define the array_write function with M_AXI pragmas
void array_write(WEIGHT_TYPE A[WEIGHT_ARRAY_SIZE], ap_uint<32> *ctrl, ap_uint<1> start) {
#pragma HLS INTERFACE m_axi depth=WEIGHT_ARRAY_SIZE offset=slave port=A
#pragma HLS INTERFACE s_axilite port=ctrl bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=start bundle=CTRL_BUS
#pragma HLS INTERFACE s_axilite port=return bundle=CTRL_BUS

    if (start) {
        for (int i = 0; i < WEIGHT_ARRAY_SIZE; i++) {
#pragma HLS PIPELINE off
            A[i] = static_cast<WEIGHT_TYPE>(i + 1);
        }
        // Optionally, you can signal the completion to the control bus here.
        *ctrl = 1;
    }
}
////////////////////////////////////////////////////////////////////////////////////////Testbench code 
#include <iostream>
#include <fstream>
#include "ap_fixed.h"
typedef ap_fixed<32, 8> WEIGHT_TYPE; // Define ap_fixed with 32 bits total (including 16 fractional bits)
const int ARRAY_SIZE = 200000; // Define the size of the array

void array_write(WEIGHT_TYPE A[ARRAY_SIZE]);

int main() {
    // Create an array to store the weights
    WEIGHT_TYPE weights[ARRAY_SIZE];

    // Call the array_write function to initialize the weights
    array_write(weights);


    // Open an input file stream to read additional weights from a file
    std::ifstream inputFile("weights.txt");

    if (!inputFile.is_open()) {
        std::cerr << "Error: Failed to open the input file." << std::endl;
        return 1;
    }

    // Read weights from the file and store them in the A array
    WEIGHT_TYPE A[ARRAY_SIZE];
    int index = 0;
    double weight;
    while (inputFile >> weight) {
        if (index < ARRAY_SIZE) {
            A[index] = static_cast<WEIGHT_TYPE>(weight);
            index++;
        } else {
            std::cerr << "Warning: Input file contains more data than the array size." << std::endl;
            break;
        }
    }

    // Close the input file stream
    inputFile.close();

    // Print the weights stored in the A array
    for (int i = 0; i < ARRAY_SIZE; i++) {
        std::cout << "A[" << i << "] = " << A[i] << std::endl;
    }

    return 0;
}
