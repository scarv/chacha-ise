
module chacha_ise_v2 (
input  wire [63:0]  rs1,
input  wire [63:0]  rs2,

input  wire         op_ad,
input  wire         op_bd,
input  wire         op_bc,

output wire [63:0]  rd
);

                            // op_bd; op_ad; op_bc
wire [31:0] a = rs1[63:32]; //  a       a     na
wire [31:0] d = rs1[31: 0]; //  d       d     nd
wire [31:0] b = rs2[63:32]; //  b      ib      b
wire [31:0] c = rs2[31: 0]; //  c      id      c

//op_ad: id<<<16
wire [31:0] c_rol_16 = {c[15:0],c[31:16]};
//op_bc: nd<<<24
wire [31:0] d_rol_24 = {d[ 7:0],d[31: 8]};

wire [31:0] xor0_lhs = op_ad ? d       : 
                     /*op_bc*/ a       ;
wire [31:0] xor0_rhs = op_ad ? c_rol_16:
                     /*op_bc*/ d_rol_24;
// op_ad:  d ^ id<<<16 
// op_bc: na ^  d<<<24 
wire [31:0] xor0_out = xor0_lhs ^ xor0_rhs;     

wire [31:0] add0_lhs = (op_bc|op_ad)? xor0_out :
                       /*     op_bd*/ a        ; 
wire [31:0] add0_rhs = (op_bd|op_ad)? b :
                       /*     op_bc*/ c ;
// op_bd:       a            +  b
// op_ad: na = (d ^ id<<<16) + ib
// op_bc:  t =  d<<<24 ^ na  +  c  
wire [31:0] add0_out = add0_lhs + add0_rhs; 

wire [31:0] xor1_lhs = op_bd ? d :
                       op_ad ? c :
                     /*op_bc*/ b ;
wire [31:0] xor1_rhs = add0_out;

// op_bd:  d ^ (a+b)
// op_ad: id ^ na
// op_bc:  b ^ t
wire [31:0] xor1_out = xor1_lhs ^ xor1_rhs; 

// op_ad: nd= (id ^    na)<<<8 
// op_bc:     ( t ^     b)<<<12
// op_bd: id= ( d ^ (a+b))<<<16   
wire [31:0] xor1_rol_8  = {xor1_out[23:0],xor1_out[31:24]};   
wire [31:0] xor1_rol_12 = {xor1_out[19:0],xor1_out[31:20]};
wire [31:0] xor1_rol_16 = {xor1_out[15:0],xor1_out[31:16]}; 


wire [31:0] add1_lhs = op_bd ? c          : 
                     /*op_bc*/ d          ;
wire [31:0] add1_rhs = op_bd ? xor1_rol_16: 
                     /*op_bc*/ add0_out   ;
// op_bd:       c + id
// op_bc: nc = nd +  t
wire [31:0] add1_out = add1_lhs + add1_rhs; 

wire [31:0] xor2_lhs = add1_out ;
wire [31:0] xor2_rhs = op_bd ? b          : 
                     /*op_bc*/ xor1_rol_12;
// op_bd: (c + id) ^ b
// op_bc: (nc ^ (t ^ b)<<<12)
wire [31:0] xor2_out = xor2_lhs ^ xor2_rhs; 

// op_bd: ib= ( b ^ (c + id)    )<<<12 
// op_bc: nb= (nc ^ (t ^ b)<<<12)<<<7
wire [31:0] xor2_rol_12 = {xor2_out[19:0],xor2_out[31:20]}; 
wire [31:0] xor2_rol_7  = {xor2_out[24:0],xor2_out[31:25]};

wire [31:0] rd_hi   = op_bd ? xor2_rol_12 : 
                      op_ad ? add0_out    :
                   /* op_bc */xor2_rol_7  ;                   

wire [31:0] rd_lo   = op_bd ? xor1_rol_16 :
                      op_ad ? xor1_rol_8  :
                   /* op_bc */add1_out    ;

assign      rd      = {rd_hi, rd_lo};

endmodule
