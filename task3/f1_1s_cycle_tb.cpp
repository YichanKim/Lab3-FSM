#include "Vf1_1s_cycle.h"
#include "verilated.h"
#include "verilated_vcd_c.h"

#include "../vbuddy.cpp" // include vbuddy code
#define MAX_SIM_CYC 100000

int main(int argc, char **argv, char **env)
{
    int simcyc;     // simulation clock count
    int tick;       // each clk cycle has two ticks for two edges
    int lights = 0; // state to toggle LED lights

    Verilated::commandArgs(argc, argv);
    // init top verilog instance
    Vf1_1s_cycle *top = new Vf1_1s_cycle;
    // init trace dump
    Verilated::traceEverOn(true);
    VerilatedVcdC *tfp = new VerilatedVcdC;
    top->trace(tfp, 99);
    tfp->open("f1_1s_cycle.vcd");

    // init Vbuddy
    if (vbdOpen() != 1)
        return (-1);
    vbdHeader("L3T2: f1 1sec cycle");
    vbdSetMode(0); // Flag mode set to continuous (not one-shot)

    // initialize simulation inputs
    top->clk = 1;
    top->rst = 0;
    top->en = 0;
    top->n_val = 24;

    // run simulation for MAX_SIM_CYC clock cycles
    for (simcyc = 0; simcyc < MAX_SIM_CYC; simcyc++)
    {
        // dump variables into VCD file and toggle clock
        for (tick = 0; tick < 2; tick++)
        {
            tfp->dump(2 * simcyc + tick);
            top->clk = !top->clk;
            top->eval();
        }

        top->rst = (simcyc < 2); // assert reset for 1st cycle
        top->en = vbdFlag(); //en is based on vbd flag
        vbdBar(top->data_out & 0xFF); //light setting

        if (Verilated::gotFinish() || vbdGetkey() == 'q')
            exit(0);
    }

    vbdClose(); // ++++
    tfp->close();
    exit(0);
}
