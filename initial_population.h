#include "systemc.h"
#include <random>
SC_MODULE(initial_population){
    // -------- inputs/outputs --------
    sc_in<bool> start;
    sc_inout<bool> initial_done;
    sc_in<double> weights[9], prices[9];
    sc_out<double> population[10][9];
    
    void start_ES_prc(){
        double percent = 0;
        for (int chromosome = 0; chromosome < 10; chromosome++) {
            for (int dimension = 0; dimension < 9; dimension++) {
                // a random number between 0 and 99 is generated
                // representing pecentage of dimension in each chromosome
                std::random_device rd;  // Obtain a random number from hardware
                std::mt19937 gen(rd()); // Seed the generator
                std::uniform_int_distribution<> distr(0, 100); // Define the range
                percent = distr(gen);
                population[chromosome][dimension].write((percent / 100.0) * weights[dimension]->read());
            }
        }
        initial_done.write(!initial_done.read());
    
    }

    SC_CTOR(initial_population){
        SC_METHOD(start_ES_prc);
        sensitive << start.pos();
    }
};