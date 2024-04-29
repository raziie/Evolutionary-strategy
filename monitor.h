#include "systemc.h"
#include <iostream>
#include <iomanip>

SC_MODULE (monitor) {
    // --------    inputs     --------
    sc_in<double> limit_sc, weights_sc[9], prices_sc[9];
    sc_in<double> fitness_sc[10], generation_sc[10][9], sorted_generation_sc[10][9];
    sc_in<double> last_best[9], new_best[9], next_generation_sc[10][9];
    sc_in<double> selected_parents[8][9];
    sc_in<bool> initial_done, eval_done, check_is_done_sc, select_done, creat_done, mute_done, repair_done;
    sc_in<double>  mutated_sc[10][9], mutated_dev_sc[9];
    sc_in<double> repaired_gen_sc[10][9];
    sc_in<int> consecutive_counter_sc;
    sc_in<bool> end_ES_sc;

    void initial_monitor(){
        while (true) {
            wait();
            cout << "\nweights:" ;
            for (int i = 0; i < 9; i++) {
                cout << std::setw(8) << weights_sc[i].read();
            }
            cout << "\nPrices:" ;
            for (int i = 0; i < 9; i++) {
                cout << std::setw(8) << prices_sc[i].read();
            }
            cout << "\n";
            
            cout << "\t------------------------ population------------------------\n";
            print_generation();
        }
    };

    void evaluation_monitor(){
        wait();
        while (true) {
            wait();
            cout << "\n\t---------------sorted population after evaluation---------------\n";
            print_sorted_generation();
        }
    };

    
    void check_monitor(){
        wait();
        while (true) {
            wait();
            cout << "\n\n\t---------------------- check -----------------------\n";
            cout << "\tend ES: " << end_ES_sc << endl;
            cout << "\tconsecutive counter: " << consecutive_counter_sc.read() << endl;
            cout << "\tnew best:";
            for (int i = 0; i < 9; i++){
                cout << new_best[i] << ", ";
            }
            cout << endl << endl;
        }
    };

    void selection_monitor(){
        while (true) {
            wait();
            cout << "\t------------------------ selected parents ------------------------\n";
            print_selected_parents();
        }
    };

    void next_gen_monitor(){
        while (true) {
            wait();
            cout << "\t------------------------ next generation ------------------------\n";
            print_next_generation();
        }
    };

    void mutation_monitor(){
        wait();
        while (true) {
            wait();
            cout << "\t------------------------ mutated generation ------------------------\n";
            print_mutated_generation();
            cout << "\t------------------------ deviations ------------------------\n";
            print_deviations();
        }
    };

    void repair_monitor(){
        while (true) {
            wait();
            cout << "\n\t------------------------ repaired generation ------------------------\n";
            print_repaired_generation();
        }
    };

    void print_generation(){
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << generation_sc[i][j];
            }
            cout << "\n" ;
        }
    }

    void print_sorted_generation(){
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << sorted_generation_sc[i][j].read();
            }
            cout << " => fitness: " << fitness_sc[i].read() << endl;
        }
    }

    void print_selected_parents(){
        for (int i = 0; i < 8; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << selected_parents[i][j];
            }
            cout << " => " << "parent" << i << endl;
        }
    }

    void print_next_generation(){
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << next_generation_sc[i][j];
            }
            if (i<4) {
                cout << " => " << "p" << i << "+" << "p" << i+4 << endl;
            }else if (i<8) {
                cout << " => " << "p" << i-4 << "+" << "p" << i << endl;
            }else {
                cout << " => best parent" << endl;
            }
        }
    }

    void print_mutated_generation(){
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << mutated_sc[i][j];
            }
            cout << endl;
        }
    }

    void print_deviations(){
        cout << "\nmutated deviation: ";
        for (int i = 0; i < 9; i++) {
            cout << mutated_dev_sc[i] << ", ";
        }
        cout << endl;
    }

    void print_repaired_generation(){
        for (int i = 0; i < 10; i++) { 
            for (int j = 0; j < 9; j++) {
                cout << std::setw(8) << repaired_gen_sc[i][j];
            }
            cout << endl;
        }
    }

    SC_CTOR (monitor) {
        SC_THREAD (initial_monitor);
        sensitive << initial_done;

        SC_THREAD (evaluation_monitor);
        sensitive << eval_done;

        SC_THREAD (selection_monitor);
        sensitive << select_done;

        SC_THREAD (next_gen_monitor);
        sensitive << creat_done;

        SC_THREAD (mutation_monitor);
        sensitive << mute_done;

        SC_THREAD (repair_monitor);
        sensitive << repair_done;

        SC_THREAD (check_monitor);
        sensitive << check_is_done_sc;
    }
};