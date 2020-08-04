
module schacha20(
input  wire [63:0]  rs1     ,
input  wire [63:0]  rs2     ,

input  wire         op_i1   ,
input  wire         op_i2   ,
input  wire         op_i3   ,
input  wire         hi      ,

output wire [63:0]  rd
);

`define ROL32(x,y) ((x<<y) | (x >> (32-y)))

wire [31:0] a = rs1[63:32];
wire [31:0] b = rs1[31: 0];
wire [31:0] c = rs2[63:32];
wire [31:0] d = rs2[31: 0];


wire [31:0] add_lhs = op_i2 ? a : c;
wire [31:0] add_rhs = op_i2 ? d : b;
wire [31:0] add_out = add_lhs + add_rhs;

wire [31:0] xor_rhs = op_i1 ? d : b;
wire [31:0] xor_out = add_out ^ xor_rhs;


wire [31:0] rot_i1_hi = `ROL32(xor_out, 16);
wire [31:0] rot_i1_lo = `ROL32(xor_out,  8);
wire [31:0] rot_i2_hi = `ROL32(xor_out, 12);
wire [31:0] rot_i2_lo = `ROL32(xor_out,  7);

wire [63:0] result_op_i3 = {d, add_out};

wire [63:0] result_op_i2 = hi ? {rot_i2_hi, add_out}    :
                                {rot_i2_lo, add_out}    ;

wire [63:0] result_op_i1 = hi ? {rot_i1_hi,       c}    :
                                {rot_i1_lo,       c}    ;

assign rd = op_i1 ? result_op_i1 :
            op_i2 ? result_op_i2 :
                    result_op_i3 ;
endmodule
