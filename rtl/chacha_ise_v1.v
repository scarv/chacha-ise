module chacha_ise_v1 (
input  wire [63:0]  rs1,
input  wire [63:0]  rs2,

input  wire         op_ad,
input  wire         op_bc,

output wire [63:0]  rd
);

wire [31:0] a = rs1[63:32];
wire [31:0] d = rs1[31: 0];
wire [31:0] b = rs2[63:32];
wire [31:0] c = rs2[31: 0];

// op_bc: ROL32(nd,24) 
wire [31:0] d_rol_24 = {d[7:0],d[31:8]};

// op_ad: ia = a + b
wire [31:0] add0_out = a + b;     

wire [31:0] xor0_lhs = op_ad ? add0_out: a;
wire [31:0] xor0_rhs = op_ad ? d       : d_rol_24;
// op_bc: id = d<<<24 ^ a
wire [31:0] xor0_out = xor0_lhs ^ xor0_rhs;     
// op_ad: id = (ia^b)<<<16   
wire [31:0] xor0_rol_16 = {xor0_out[15:0],xor0_out[31:16]}; 


wire [31:0] add1_lhs = c;
wire [31:0] add1_rhs = op_ad ? xor0_rol_16: xor0_out;
// op_ad: ic = c +id 
// op_bc: ic = c +id
wire [31:0] add1_out = add1_lhs + add1_rhs;    

wire [31:0] xor1_lhs = b;
wire [31:0] xor1_rhs = add1_out;
wire [31:0] xor1_out = xor1_lhs ^ xor1_rhs;
// op_ad: ib = (b^ic)<<<12
// op_bc: ib = (b^ic)<<<12
wire [31:0] xor1_rol_12 = {xor1_out[19:0],xor1_out[31:20]};

wire [31:0] add2_lhs = op_ad ? add0_out   : add1_out;
wire [31:0] add2_rhs = op_ad ? xor1_rol_12: d;
// op_ad: na = ia+ib 
// op_bc: nc = ic+ d
wire [31:0] add2_out = add2_lhs + add2_rhs;    

wire [31:0] xor2_lhs = add2_out;
wire [31:0] xor2_rhs = op_ad ? xor0_rol_16 : xor1_rol_12;
// op_ad: nd = na^id 
// op_bc: nb = nc^ib
wire [31:0] xor2_out = xor2_lhs ^ xor2_rhs;

// op_ad: nd = (na^id)<<<8
wire [31:0] xor2_rol_8 = {xor2_out[23:0],xor2_out[31:24]};
// op_bc: nb = (nc^ib)<<<7
wire [31:0] xor2_rol_7 = {xor2_out[24:0],xor2_out[31:25]};

wire [31:0] rd_hi   = op_ad ? add2_out   : xor2_rol_7;
wire [31:0] rd_lo   = op_ad ? xor2_rol_8 : add2_out;

assign      rd      = {rd_hi, rd_lo};

endmodule
