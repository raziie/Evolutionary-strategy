#include "systemc.h"

SC_MODULE(repair){
    // input generation
    sc_in<double> mutated_generation[10][9];
    sc_inout<bool> repair_done;
    sc_in<bool> mute_done;
    // output generation
    sc_out<double> repaired_generation[10][9];
    sc_in<double> weights[9];
    
    void repair_ES_prc(){
        for (int i=0; i<10; i++) {
            for (int j=0; j<9; j++) {
                if(mutated_generation[i][j] < 0) {
                    repaired_generation[i][j].write(0);
                } else if(mutated_generation[i][j] > weights[j]){
                    repaired_generation[i][j].write(weights[j]);
                } else {
                    repaired_generation[i][j].write(mutated_generation[i][j]);
                }
            }
        }
        repair_done.write(!repair_done.read());
    }

    SC_CTOR(repair){
        SC_METHOD(repair_ES_prc);
        sensitive << mute_done;
    }
};