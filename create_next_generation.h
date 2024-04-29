#include "systemc.h"
#include <iostream>
#include <random>
#include <vector>
#define PCO 98

SC_MODULE(create_next_generation){
    // -------- inputs/outputs --------
    sc_in<double> sorted_generation[10][9];
    sc_in<double> selected_parents[8][9];
    sc_out<double> next_generation[10][9];
    sc_inout<bool> creat_done;
    sc_in<bool> select_done;
    // -------- Local variables --------
    double probabilities [10];

    void create_next_generation_prc(){
        // combination of 4 pairs
        // pairs indexes: (0, 4), (1, 5), (2, 6), (3, 7)
        for (int pair = 0; pair < 4; pair++) {
            std::random_device rd;  // Obtain a random number from hardware
            std::mt19937 gen(rd()); // Seed the generator
            std::uniform_int_distribution<> distr(0, 99); // Define the range (Pco)
            std::uniform_int_distribution<> cross(1, 9); // Define the range for cross point
            
            // a random number between 0 and 99 is generated
            double randomDouble = distr(gen);

            // if that random number is less than pco=98 then combination will happen 
            if (randomDouble < PCO){
                // pick a random number as cross point between 1 to 9
                int crosspoint = cross(gen);

                int j = 0;
                // switch dimension values up to crosspoint
                for (; j < crosspoint ; j++) {
                    next_generation[pair][j].write(selected_parents[pair+4][j]);
                    next_generation[pair+4][j].write(selected_parents[pair][j]);
                }
                // add the remaining as it is
                for (; j < 9 ; j++) {
                    next_generation[pair][j].write(selected_parents[pair][j]);
                    next_generation[pair+4][j].write(selected_parents[pair+4][j]);
                }
            } else {
                // comb did not happen
                for (int j=0; j < 9 ; j++) {
                    next_generation[pair][j].write(selected_parents[pair][j]);
                    next_generation[pair+4][j].write(selected_parents[pair+4][j]);
                }
            }
        }
        // eilitism
        // move two best chromosomes directly to nex generation
        for (int i=8; i<10; i++) {
            for (int j=0; j<9; j++) {
                next_generation[i][j].write(sorted_generation[i][j]);
            }
        }

        creat_done.write(!creat_done.read());
    };

    SC_CTOR(create_next_generation){
        SC_METHOD(create_next_generation_prc);
        sensitive << select_done;
    };
};