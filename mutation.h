#include <iostream>
#include <iterator>
#include <random>
#include "systemc.h"

SC_MODULE(mutation){
    // input chromosome
    sc_in<double> generation[10][9];
    // input sigmas (sigma j)
    sc_in<int> first_five;
    // mutated chromosome
    sc_out<double> mutated[10][9];
    // mutated sigmas (sigma j)
    sc_in<bool> create_done;
    sc_out<double> mutated_dev[9];
    sc_inout<bool> mute_done;
    // local
    double temp = 0;
    double devs[9];

    void mutate_ES_prc(){
        for (int i = 0; i < 9; i++){
            if (first_five.read() <= 1){
                devs[i] = 1;
            }
        }
        
        // Define random generator with Gaussian distribution
        cout  << endl;
        const double mean = 0.0;
        const double stddev = 1.0;
        std::default_random_engine generator;
        std::normal_distribution<double> dist(mean, stddev);

        // Add Gaussian noise
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                // xj = xj + (N(0, 1) * sigmaj)
                if (i<8){
                    temp = (generation[i][j].read() + (dist(generator) * devs[j]));
                    mutated[i][j] = floor(temp * 10) / 10;
                }else{
                    mutated[i][j] = generation[i][j]->read();
                }
            }
        }

        // common between all dimensions N(0, 1)
        double common_rand = dist(generator);
        // Add noise to deviations of dimensions
        for (int i = 0; i < 9; i++) {
            // dist(generator) is Nj(0, 1): rand number for dimension j
            temp = (devs[i] * exp(common_rand - dist(generator)));
            mutated_dev[i] = floor(temp * 10) / 10;
            devs[i] = floor(temp * 10) / 10;
        }
        mute_done.write(!mute_done.read());
    }

    SC_CTOR(mutation){
        SC_METHOD(mutate_ES_prc);
        sensitive << create_done;
    }
};