#include <iostream>
#include "bnn.h"
#include "golden.h"

int main() {

    int correct = 0;

    for (int i = 0; i < NUM_SAMPLES; i++) {

        // Prepare input buffer
        DTYPE IN[784];
        for (int j = 0; j < 784; j++) {
            IN[j] = inputs[i][j]; 
        }

        // Run BNN
        ITYPE ys[10];
        bnn(IN, ys);

        // Compare with golden output
        bool match = true;
        for (int k = 0; k < 10; k++) {
            if (ys[k] != golden_outputs[i][k]) {
                match = false;
            }
        }

        if (match) correct++;

        std::cout << "Sample " << i << "  result = [ ";
        for (int k = 0; k < 10; k++) 
            std::cout << ys[k] << " ";

        std::cout << "]  (golden ";
        for (int k = 0; k < 10; k++) 
            std::cout << golden_outputs[i][k] << " ";
        std::cout << ")  => " << (match ? "OK" : "FAIL") << std::endl;
    }

    std::cout << "\nSummary: " << correct << "/" << NUM_SAMPLES << " matched golden results.\n";

    return 0;
}
