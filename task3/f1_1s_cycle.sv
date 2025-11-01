module f1_1s_cycle (
    input logic             en,
    input logic             rst,
    input logic             clk,
    input logic [15:0]      n_val,
    output logic [7:0]      data_out
);

logic tick; //internal connection between tick output and en input

clktick one_second_tick(
    .clk(clk),
    .rst(rst),
    .en(en),
    .N(n_val),
    .tick(tick)
);

f1_fsm f1_light_sequence(
    .rst(rst),
    .en(tick),
    .clk(clk),
    .data_out(data_out)
);

endmodule
