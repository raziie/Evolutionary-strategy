#include "systemc.h"

SC_MODULE(evaluation){
    // -------- inputs/outputs --------
    sc_in<int> first_five;
    sc_in<double> limit;    // size of the bag
    sc_in<double> generation[10][9], repaired_generation[10][9];    // input chromosome
    sc_in<double> weights[9], prices[9];
    sc_out<double> new_best[9], fitness_val[10];     // fitness value
    sc_out<double> sorted_generation[10][9];
    sc_inout<bool>  eval_done;
    sc_in<bool> initial_done;
    sc_in<bool> repair_done;
    // -------- local variables --------
    double weight;
    double price;
    double local_fit_vals[10];
    double disordered_generation[10][9];
    double current_generation[10][9];

    void fitness_ES_prc(){
        // main loop to get fitness of all chromosomes in generation
        for (int i = 0; i < 10; i++) {  
            for (int j = 0; j < 9; j++) {
                if (first_five.read() == 1) {
                    current_generation[i][j] = generation[i][j].read();
                } else {
                    current_generation[i][j] = repaired_generation[i][j].read();
                }
            }
        }      

        for (int i = 0; i < 10; i++) {    
            weight = 0;
            for (int j = 0; j < 9; j++) {
                weight =  weight + current_generation[i][j];
                disordered_generation[i][j] = (current_generation[i][j]);
            }

            if (weight > limit.read()) {
                local_fit_vals[i] = -1;
            } else {
                price = 0;
                for (int t = 0; t < 9; t++) {
                    price = price + current_generation[i][t] * (prices[t].read() / weights[t].read());
                }
                local_fit_vals[i] = price;
            }
        }

        // sort generation based on fitness
        for (int i = 0; i < 10 - 1; i++) {
            for (int j = 0; j < 10 - i - 1; j++) {
                if (local_fit_vals[j] > local_fit_vals[j + 1]) {
                    swap(j, j + 1);
                }
            }
        }
        //  write values in (sorted generation, fitness array, best_cromosom) and print
        for (int i = 0; i < 10; i++) {    
            for (int j = 0; j < 9; j++) {
                sorted_generation[i][j].write(disordered_generation[i][j]);
                if (i == 9){
                    new_best[j].write(disordered_generation[i][j]);
                }
            }
            fitness_val[i].write(local_fit_vals[i]);
        }
        eval_done.write(!eval_done.read());
    }

    void swap(int t, int k){
        double temp;
        // swap fitness
        temp = local_fit_vals[k];
        local_fit_vals[k] = local_fit_vals[t];
        local_fit_vals[t] = temp;
        // swap cromosome
        for (int u = 0; u < 9; u++) {
            temp = disordered_generation[t][u];
            disordered_generation[t][u] = disordered_generation[k][u];
            disordered_generation[k][u] = temp;
        }
    }

    SC_CTOR(evaluation){
        SC_METHOD(fitness_ES_prc);
        sensitive << initial_done << repair_done;
    }
};