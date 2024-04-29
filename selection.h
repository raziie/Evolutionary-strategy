#include "systemc.h"
#include <iostream>
#include <random>
#include <vector>
#define PCO 98

SC_MODULE(selection){
    // -------- inputs/outputs --------
    sc_in<double> sorted_generation[10][9];
    sc_in<double> fitness[10];
    sc_out<double> selected_parents[8][9];
    sc_inout<bool> select_done;
    sc_in<bool> check_is_done;
    // -------- Local variables --------
    double probabilities [10];

    void selection_prc(){
        for (int i = 0; i < 10; i++) { 
            probabilities[i] = fitness[i].read();
        }
        // find 8 parents to generate 8 children for next generation
        for (int i = 0; i < 8; i++) { 
            // generate a random index 
            // more qualified ones have more chance to be selected 
            std::discrete_distribution<int> distribution(probabilities, probabilities + 10);
            std::random_device rd;
            std::mt19937 generator(rd());
        
            int randomIndex = distribution(generator);

            // copy items of parent with random index to selected_parents
            for (int j = 0; j < 9; j++) {
                selected_parents[i][j].write(sorted_generation[randomIndex][j]);
            }
        }
        select_done.write(!select_done);
    };

    SC_CTOR(selection){
        SC_METHOD(selection_prc);
        sensitive << check_is_done;
    };
};
