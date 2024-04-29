#include "driver.h"
#include "monitor.h"
#include "initial_population.h"
#include "evaluation.h"
#include "check_condition_loop.h"
#include "selection.h"
#include "create_next_generation.h"
#include "mutation.h"
#include "repair.h"

int sc_main(int argc, char* argv[]) {
  sc_signal<bool> initial_done, eval_done, check_is_done_sc, select_done, creat_done, mute_done, repair_done;
  // initial population
  sc_signal<double> weights_sc[9], prices_sc[9], generation_sc[10][9];
  sc_signal<bool> init_st_sc;
  // evaulation
  sc_signal<double> limit_sc, fitness_sc[10], sorted_generation_sc[10][9];
  limit_sc = 25;
  // check loop condition
  sc_signal<double> new_best_sc[9], last_best_sc[9];
  sc_signal<int> first_five_times_sc, consecutive_counter_sc;
  sc_signal<bool> end_ES_sc;
  // selection
  sc_signal<double> selected_parents_sc[8][9];
  // create next generation
  sc_signal<double> next_generation_sc[10][9];
  // mutation
  sc_signal<double> mutated_sc[10][9], mutated_dev_sc[9];
  //  deviation_sc[9],
  // repair
  sc_signal<double> repaired_gen_sc[10][9];
  

  //  driver
  driver d ("Driver");
  d.init_st_sc(init_st_sc);
  d.end_ES(end_ES_sc);
  for (int i = 0; i < 9; i++) {
    d.weights[i](weights_sc[i]);
    d.prices[i](prices_sc[i]);
  }
 
  //  initial_population
  initial_population pop ("InitialPopulation");
  pop.start(init_st_sc);
  for (int i = 0; i < 9; i++) {
    pop.weights[i](weights_sc[i]);
    pop.prices[i](prices_sc[i]);
    for (int j = 0; j < 10; j++) {
      pop.population[j][i](generation_sc[j][i]);
    }
  }
  pop.initial_done(initial_done);
  
  //  evaluation
  evaluation eval ("Evaluation");
  eval.limit(limit_sc);
  eval.first_five(first_five_times_sc);
  for (int j = 0; j < 9; j++){
    for (int i = 0; i < 10; i++){
      eval.generation[i][j](generation_sc[i][j]);  
      eval.sorted_generation[i][j](sorted_generation_sc[i][j]);  
      eval.repaired_generation[i][j](repaired_gen_sc[i][j]); 
    }
    eval.fitness_val[j](fitness_sc[j]);
    eval.weights[j](weights_sc[j]);
    eval.prices[j](prices_sc[j]);
    eval.new_best[j](new_best_sc[j]);
  }
  eval.fitness_val[9](fitness_sc[9]);
  eval.eval_done(eval_done);
  eval.initial_done(initial_done);
  eval.repair_done(repair_done);

  // check_condion_loop
  check_condition_loop condion_loop("CheckLoopCondition");
  for (int i = 0; i < 9; i++){
    condion_loop.new_best[i](new_best_sc[i]);
    condion_loop.last_best[i](last_best_sc[i]);
  }
  condion_loop.first_five_times(first_five_times_sc);
  condion_loop.consecutive_counter(consecutive_counter_sc);
  condion_loop.end_ES(end_ES_sc);
  condion_loop.check_is_done(check_is_done_sc);
  condion_loop.eval_done(eval_done);

  // selection
  selection select ("Selction");
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 9; j++){
      select.sorted_generation[i][j](sorted_generation_sc[i][j]);
      if (i < 8){
        select.selected_parents[i][j](selected_parents_sc[i][j]);
      }
    }
    select.fitness[i](fitness_sc[i]);
  }
  select.select_done(select_done);
  select.check_is_done(check_is_done_sc);

  // create next generation
  create_next_generation next_gen ("NextGeneration");
  for (int i = 0; i < 10; i++){
    for (int j = 0; j < 9; j++){
      next_gen.sorted_generation[i][j](sorted_generation_sc[i][j]);
      next_gen.next_generation[i][j](next_generation_sc[i][j]);
      if (i < 8){
        next_gen.selected_parents[i][j](selected_parents_sc[i][j]);
      }
    }
  }
  next_gen.creat_done(creat_done);
  next_gen.select_done(select_done);

  //mutation
  mutation mut ("mut");
  for (int i = 0; i < 9; i++) {
    mut.mutated_dev[i](mutated_dev_sc[i]);
    for (int j = 0; j < 10; j++) {
      mut.mutated[j][i](mutated_sc[j][i]);
      mut.generation[j][i](next_generation_sc[j][i]);
    }
  }
  mut.first_five(first_five_times_sc);
  mut.mute_done(mute_done);
  mut.create_done(creat_done);

  //repair
  repair repair ("repair");
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 10; j++) {
      repair.mutated_generation[j][i](mutated_sc[j][i]);
      repair.repaired_generation[j][i](repaired_gen_sc[j][i]);
    }
    repair.weights[i](weights_sc[i]);
  }
  repair.repair_done(repair_done);
  repair.mute_done(mute_done);

  // monitor;
  monitor mo ("Monitor");
  mo.limit_sc(limit_sc);
  for (int j = 0; j < 9; j++){
    for (int i = 0; i < 10; i++){
      mo.generation_sc[i][j](generation_sc[i][j]);  
      mo.sorted_generation_sc[i][j](sorted_generation_sc[i][j]);   
      mo.next_generation_sc[i][j](next_generation_sc[i][j]);
      mo.mutated_sc[i][j](mutated_sc[i][j]);
      mo.repaired_gen_sc[i][j](repaired_gen_sc[i][j]);
      if (i < 8){
        mo.selected_parents[i][j](selected_parents_sc[i][j]);
      }
    }
    mo.fitness_sc[j](fitness_sc[j]);
    mo.weights_sc[j](weights_sc[j]);
    mo.prices_sc[j](prices_sc[j]);
    mo.new_best[j](new_best_sc[j]);
    mo.last_best[j](last_best_sc[j]);
    mo.mutated_dev_sc[j](mutated_dev_sc[j]);
  }
  mo.fitness_sc[9](fitness_sc[9]);
  mo.consecutive_counter_sc(consecutive_counter_sc);
  mo.end_ES_sc(end_ES_sc);
  
  mo.initial_done(initial_done);
  mo.eval_done(eval_done);
  mo.check_is_done_sc(check_is_done_sc);
  mo.select_done(select_done);
  mo.creat_done(creat_done);
  mo.mute_done(mute_done);
  mo.repair_done(repair_done);

  sc_start();
  return (0);
}
