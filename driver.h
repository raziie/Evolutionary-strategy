#include "systemc.h"

SC_MODULE (driver) {
    // --------     outputs     --------
    sc_out<bool> init_st_sc;
    sc_out<double> weights[9], prices[9];
    sc_in<bool> end_ES;

    // -------- local variables --------
    double lw[9] = {2,3,6,7,5,9,3,4,5};
    double lp[9] = {6,5,8,9,6,7,3,6,8};
    
    void prc_driver () {
        // initial values
        for (int i = 0; i < 9; i++) {
            weights[i].write(lw[i]);
            prices[i].write(lp[i]);
        }
        init_st_sc.write(false);
        wait(200, SC_NS);
        init_st_sc.write(true);

        wait(100, SC_NS);

        init_st_sc.write(false);
        wait(10, SC_SEC);
    };

    SC_CTOR (driver) {
        SC_THREAD (prc_driver);
    }
};