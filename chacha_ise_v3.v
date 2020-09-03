
module chacha_ise_v3 (
input  wire [63:0]  rs1     ,
input  wire [63:0]  rs2     ,

input  wire         op_ad0  ,
input  wire         op_bc0  ,
input  wire         op_ad1  ,
input  wire         op_bc1  ,

output wire [63:0]  rd
);

wire [31:0] a = rs1[63:32];
wire [31:0] b = rs2[63:32];
wire [31:0] c = rs2[31: 0];
wire [31:0] d = rs1[31: 0];

wire    ad = op_ad0 || op_ad1;
wire    bc = op_bc0 || op_bc1;

wire [31:0] add_lhs = ad ? a : c;
wire [31:0] add_rhs = ad ? b : d;
wire [31:0] add_out = add_lhs + add_rhs;

wire [31:0] xor_rhs = ad ? d : b;
wire [31:0] xor_out = add_out ^ xor_rhs;

wire [ 4:0] rol_amnt= op_ad0 ? 16   :
                      op_bc0 ? 12   :
                      op_ad1 ?  8   :
                   /* op_bc1*/  7   ;

wire [31:0] rol_ad0 = {xor_out[15:0],xor_out[31:16]};
wire [31:0] rol_bc0 = {xor_out[19:0],xor_out[31:20]};
wire [31:0] rol_ad1 = {xor_out[23:0],xor_out[31:24]};
wire [31:0] rol_bc1 = {xor_out[24:0],xor_out[31:25]};
wire [31:0] rol_out = op_ad0 ? rol_ad0  :
                      op_bc0 ? rol_bc0  :
                      op_ad1 ? rol_ad1  :
                   /* op_bc1*/ rol_bc1  ;

wire [31:0] rd_hi   = ad ? add_out : rol_out;
wire [31:0] rd_lo   = ad ? rol_out : add_out;

assign      rd      = {rd_hi, rd_lo};

endmodule
