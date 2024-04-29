SC_MODULE(check_condition_loop){
    // -------- inputs/outputs --------
    sc_in<double> new_best[9];
    sc_inout<double> last_best[9];
    sc_inout<int> first_five_times, consecutive_counter;
    sc_out<bool> end_ES;
    sc_out<bool> check_is_done;
    sc_in<bool>  eval_done;
    // -------- local variables --------
    bool best_is_same;
    
    void check_condition_prc(){
        best_is_same = true;
        // only check after first five iteraion
        if (first_five_times.read() == 5){
            // compare last and new chromosome then assign new to last one
            for (int i = 0; i < 9; i++){
                if (best_is_same && last_best[i].read() != new_best[i].read()){
                    best_is_same = false;
                }
                last_best[i].write(new_best[i]);
            }
            if (best_is_same){  //  if is same
                consecutive_counter.write(consecutive_counter.read() + 1);
            }else{
                consecutive_counter.write(0);
            }
            // check if 5 consequtive best cromosome was same
            if (consecutive_counter.read() == 5){
                end_ES.write(true);
                sc_abort();
            }
            
        }else{
            first_five_times.write(first_five_times.read() + 1);
        }
        check_is_done.write(!check_is_done.read());
    }

    SC_CTOR(check_condition_loop){
        SC_METHOD(check_condition_prc);
        sensitive << eval_done;
    }
};