//optimal N value is 24

module clktick #(
    parameter WIDTH = 16
)(
  // interface signals
  input  logic             clk,      // clock 
  input  logic             rst,      // reset
  input  logic             en,       // enable signal
  input  logic [WIDTH-1:0] N,     	 // clock divided by N+1
  output logic  		   tick      // tick output
);

logic [WIDTH-1:0] count;

always_ff @ (posedge clk)
    if (rst) begin                   // common practice to have being/end for all if
        tick <= 1'b0;                // output a 0
        count <= N;                  // set count back to N
        end
    else if (en) begin
        if (count == 0) begin
            tick <= 1'b1;            // set output to 1
            count <= N;              // reset count
            end
        else begin
            tick <= 1'b0;            // set output back to 0
            count <= count - 1'b1;   // decrement count by 1
            end
        end
endmodule
