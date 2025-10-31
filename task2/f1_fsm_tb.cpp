#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vf1_fsm.h"
#define MAX_SIM_CYC 1000000

#include "vbuddy.cpp" //include vbuddy code

int main(int argc, char **argv, char **env) {
  int simcyc;     // simulation clock count
  int tick;       // each clk cycle has two ticks for two edges

  Verilated::commandArgs(argc, argv);
  // init top verilog instance
  Vf1_fsm* top = new Vf1_fsm;
  // init trace dump
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace (tfp, 99);
  tfp->open ("f1_fsm.vcd");
 
  // init Vbuddy
  if (vbdOpen()!=1) return(-1);
  vbdHeader("L3T2: F1 Light Sequence");
  vbdSetMode(1);
 
  // initialize simulation inputs
  top->clk = 1;
  top->rst = 1;
  top->en = 0;

  // run simulation for MAX_SIM_CYC clock cycles
  for (simcyc=0; simcyc<MAX_SIM_CYC; simcyc++) {
    // dump variables into VCD file and toggle clock
    for (tick=0; tick<2; tick++) {
      tfp->dump (2*simcyc+tick);
      top->clk = !top->clk;
      top->eval ();
    }

    top->en = vbdFlag();
    top->rst = (simcyc < 2);

    // plot data_out from module
    vbdBar(top->data_out & 0xFF);
    
    // either simulation finished, or 'q' is pressed
    if ((Verilated::gotFinish()) || (vbdGetkey()=='q')) 
      exit(0);                // ... exit if finish OR 'q' pressed
  }

  vbdClose();     // ++++
  tfp->close(); 
  delete top;
  exit(0);
}