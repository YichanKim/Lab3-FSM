module lfsr_7 (
    input   logic       clk,
    input   logic       rst,
    input   logic       en,
    output  logic [6:0] data_out
);

logic [6:0] sreg; 

always_ff @ (posedge clk, posedge rst)
    if (rst)
        sreg <= 7'b1;
    else
        sreg <= {sreg[6:0], sreg[2] ^ sreg[6]};
assign data_out = sreg;

endmodule
