
module chacha_ise (
input  wire [63:0]  rs1,
input  wire [63:0]  rs2,

input  wire         op_add,
input  wire         op_xorrol_16,
input  wire         op_xorrol_12,
input  wire         op_xorrol_8,
input  wire         op_xorrol_7,

output wire [63:0]  rd
);

wire [31:0] a_hi = rs1[63:32];
wire [31:0] b_hi = rs2[63:32];
wire [31:0] a_lo = rs1[31: 0];
wire [31:0] b_lo = rs2[31: 0];

//wire     op_xor= op_xorrol_16 || op_xorrol_12 || op_xorrol_8 || op_xorrol_7;

//wire [31:0] add_hi = a_hi + b_hi;
//wire [31:0] add_lo = a_lo + b_lo;

wire [31:0] xor_hi = a_hi ^ b_hi;
wire [31:0] xor_lo = a_lo ^ b_lo;

wire [31:0] rol_hi_16 = {xor_hi[15:0],xor_hi[31:16]};
wire [31:0] rol_hi_12 = {xor_hi[19:0],xor_hi[31:20]};
wire [31:0] rol_hi_8  = {xor_hi[23:0],xor_hi[31:24]};
wire [31:0] rol_hi_7  = {xor_hi[24:0],xor_hi[31:25]};

wire [31:0] xorrol_hi = op_xorrol_7   ? rol_hi_7  :
                        op_xorrol_8   ? rol_hi_8  :
                        op_xorrol_12  ? rol_hi_12 :
                     /* op_xorrol_16 */ rol_hi_16 ;

wire [31:0] rol_lo_16 = {xor_lo[15:0],xor_lo[31:16]};
wire [31:0] rol_lo_12 = {xor_lo[19:0],xor_lo[31:20]};
wire [31:0] rol_lo_8  = {xor_lo[23:0],xor_lo[31:24]};
wire [31:0] rol_lo_7  = {xor_lo[24:0],xor_lo[31:25]};

wire [31:0] xorrol_lo = op_xorrol_7   ? rol_lo_7  :
                        op_xorrol_8   ? rol_lo_8  :
                        op_xorrol_12  ? rol_lo_12 :
                     /* op_xorrol_16 */ rol_lo_16 ;

//wire [31:0] rd_hi   = op_add ? add_hi : xorrol_hi;
//wire [31:0] rd_lo   = op_add ? add_lo : xorrol_lo;

//assign      rd      = {rd_hi, rd_lo};
assign        rd      = {xorrol_hi, xorrol_lo};

endmodule
