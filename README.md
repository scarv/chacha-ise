# The implementations of lightweight ISEs for ChaCha on RISC-V 

<!--- -------------------------------------------------------------------- --->

## Overview

This is a repository for the source codes of a lightweight Instruction Set Extension implementation to accelerate the ChaCha20 stream cipher on 64-bit RISC-V architectures.
The work is published in [1].


<!--- -------------------------------------------------------------------- --->

## Organisation

```
├── bin                - scripts (e.g., environment configuration)
├── emulator           - ChaCha ISE-supported Rocket-Chip Emulator and its HAL support
├── rtl                - the implementations of ChaCha ISE variances in verilog
├── src                - source code
│   ├── chacha_ref       - source code for the vanilla C implementation
│   ├── cmodel_ise       - source code for the model of ISEs in C
│   ├── cmodel_test      - source code for verifying the ISE models in the ChaCha block
│   ├── emu_eval         - source code for comparing the ISE variances in emulator
│   ├── emu_ise          - source code for ISE-assisted ChaCha encryption in emulator
│   ├── sim_rvb          - source code for the ChaCha encryption using RV64IB
│   └── sim_vec          - source code for the ChaCha encryption using vector extension
├── synth-yosys        - scripts to synthesise the ChaCha ISE using Yosys
├── toolchain-vec      - scripts to install the RISC-V toolchains for vector extension
└── work               - working directory for build

```

<!--- -------------------------------------------------------------------- --->

## Quickstart

- For seting up the repository

  - Fix path for the RISCV toolchain, e.g., 
  
    ```sh
    export RISCV="/opt/riscv"
    ```
  
  - Clone the repository and setup environment
  
    ```sh
    git clone https://github.com/scarv/chacha-ise ./chacha-ise
    cd ./chacha-ise
    git submodule update --init --recursive
    source bin/source.me.sh
    ```
  
  - Build vector-supported RISC-V toolchain 

    ```sh
    make -f toolchain-vec/Makefile clone 
    make -f toolchain-vec/Makefile build
    ```
  
- Build and execute the C model of ChaCha ISEs

  ```sh
  make cmodel_test
  ```

- Build and evaluate the ChaCha ISE variances

  ```sh
  make emu_eval
  ```

- Build and evaluate the ISE-assisted implementation.

  ```sh
  make emu_ise
  ```

- Build and evaluate the vectorised implementations

  ```sh
  make sim_vec
  ```
- Build and evaluate the implementations using the Bitmanip extension 

  ```sh
  make sim_rvb
  ```

<!--- -------------------------------------------------------------------- --->

### RV64 ChaCha ISE Design

- ChaCha V1 variant.

  - V1 is based on a performance-oriented approach to execute the quarter round with a minimal number of instructions.
  - instructions:

    ```
    chacha.ad.v1      rd, rs1, rs2   => a  <- GPR[rs1]_{63..32}
                                        d  <- GPR[rs1]_{31.. 0}
                                        b  <- GPR[rs2]_{63..32}
                                        c  <- GPR[rs2]_{31.. 0}
                                        ia <-   a +  b
                                        id <- (ia ^  d) <<< 16
                                        ic <-   c + id
                                        ib <- (ic ^  b) <<< 12
                                        na <-  ia + ib
                                        nd <- (ia ^ na) <<< 8
                                        GPR[rd] <- na || nd

    chacha.bc.v1      rd, rs1, rs2   => a  <- GPR[rs1]_{63..32}
                                        d  <- GPR[rs1]_{31.. 0}
                                        b  <- GPR[rs2]_{63..32}
                                        c  <- GPR[rs2]_{31.. 0}
                                        id <- (d <<< 24) ^ a
                                        ic <-   c + id
                                        ib <- (ic ^  b) <<< 12
                                        nc <-  ic +  d
                                        nb <- (ib ^ nc) <<< 7
                                        GPR[rd] <- nb || nc
    ```

  - encodings:

    ```
    .macro chacha.ad.v1                 rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0000000, \rd, \rs1, \rs2
    .endm
    .macro chacha.bc.v1                 rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0000001, \rd, \rs1, \rs2
    .endm
    ```

- ChaCha V2 variant.

  - V2 introduces an area-oriented approach to aim at favouring re-usage between ISEs to reduce overall area overhead.
  - instructions:

    ```
    chacha.ad0.v2     rd, rs1, rs2   => a  <- GPR[rs1]_{63..32}
                                        b  <- GPR[rs2]_{63..32}
                                        d  <- GPR[rs1]_{31.. 0}
                                        na <-   a + b
                                        nd <- (na ^ d) <<< 16
                                        GPR[rd] <- na || nd

    chacha.bc0.v2     rd, rs1, rs2,  => b  <- GPR[rs2]_{63..32}
                                        c  <- GPR[rs2]_{31.. 0}
                                        d  <- GPR[rs1]_{31.. 0}
                                        nc <-   c + d
                                        nb <- (nc ^ b) <<< 12
                                        GPR[rd] <- nb || nc

    chacha.ad1.v2     rd, rs1, rs2   => a  <- GPR[rs1]_{63..32}
                                        b  <- GPR[rs2]_{63..32}
                                        d  <- GPR[rs1]_{31.. 0}
                                        na <-   a + b
                                        nd <- (na ^ d) <<< 8
                                        GPR[rd] <- na || nd

    chacha.bc1.v2     rd, rs1, rs2,  => b  <- GPR[rs2]_{63..32}
                                        c  <- GPR[rs2]_{31.. 0}
                                        d  <- GPR[rs1]_{31.. 0}
                                        nc <-   c + d
                                        nb <- (nc ^ b) <<< 7
                                        GPR[rd] <- nb || nc
    ```

  - encodings:

    ```
    .macro chacha.ad0.v2                rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0010000, \rd, \rs1, \rs2
    .endm
    .macro chacha.bc0.v2                rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0010001, \rd, \rs1, \rs2
    .endm
    .macro chacha.ad1.v2                rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0010010, \rd, \rs1, \rs2
    .endm
    .macro chacha.bc1.v2                rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0010011, \rd, \rs1, \rs2
    .endm
    ```

- ChaCha V3 variant.

  - V 3 follows a parallel oriented approach to perform simultaneously two quarter-rounds.
  - instructions:

    ```
    chacha.add.v3  rd, rs1, rs2      => a1 <- GPR[rs1]_{63..32}
                                        a0 <- GPR[rs1]_{31.. 0}
                                        b1 <- GPR[rs2]_{63..32}
                                        b0 <- GPR[rs2]_{31.. 0}
                                        a0 <- a0 + b0
                                        a1 <- a1 + b1
                                        GPR[rd] <- a1 || a0

    chacha.xor.v3  rd, rs1, rs2, imm => d1 <- GPR[rs1]_{63..32}
                                        d0 <- GPR[rs1]_{31.. 0}
                                        a1 <- GPR[rs2]_{63..32}
                                        a0 <- GPR[rs2]_{31.. 0}
                                        ri <- DEC[imm-1]
                                        d0 <- (a0 ^ d0) <<< ri
                                        d1 <- (a1 ^ d1) <<< ri
                                        GPR[rd] <- d1 || d0
    ```

    such that

    ```
    DEC = { 16, 12,  8, 7 }
    ```

  - encodings:

    ```
    .macro chacha.add.v3                rd,  rs1,  rs2
    .insn  r   CUSTOM_0, 7, 0b0011000, \rd, \rs1, \rs2
    .endm
    .macro chacha.xor.v3                rd,  rs1,  rs2, imm
    .insn  r   CUSTOM_0, 7, 3*8+ \imm, \rd, \rs1, \rs2
    .endm
    ```

- The BitManip-like pack ISE:

  - instructions:

    ```
    rv64.packll       rd, rs1, rs2      => lo <- GPR[rs1]_{31..0}
                                           hi <- GPR[rs2]_{31..0}
                                           GPR[rd] <- hi || lo
    rv64.packhh       rd, rs1, rs2      => lo <- GPR[rs1]_{63..32}
                                           hi <- GPR[rs2]_{63..32}
                                           GPR[rd] <- hi || lo
    rv64.packhl       rd, rs1, rs2      => lo <- GPR[rs1]_{31..0}
                                           hi <- GPR[rs2]_{63..32}
                                           GPR[rd] <- hi || lo
    rv64.packlh       rd, rs1, rs2      => lo <- ( GPR[rs1]_{63..32} 
                                           hi <- ( GPR[rs2]_{31..0}
                                           GPR[rd] <- hi || lo
    ```

  - encodings:

    ```
    .macro rv64.packll                 rd,  rs1,  rs2
    .insn  r  CUSTOM_0, 7, 0b0000100, \rd, \rs1, \rs2 
    .endm
    .macro rv64.packhh                 rd,  rs1,  rs2
    .insn  r  CUSTOM_0, 7, 0b0000101, \rd, \rs1, \rs2
    .endm
    .macro rv64.packhl                 rd,  rs1,  rs2
    .insn  r  CUSTOM_0, 7, 0b0000110, \rd, \rs1, \rs2
    .endm
    .macro rv64.packlh                 rd,  rs1,  rs2
    .insn  r  CUSTOM_0, 7, 0b0000111, \rd, \rs1, \rs2
    .endm
    ```

<!--- -------------------------------------------------------------------- --->

## References

[1] Ben Marshall, Daniel Page, Thinh H. Pham, "A Lighweight ISE for ChaCha on RISCV", The 32nd IEEE International Conference on Application-specific Systems, Architectures and Processors (ASAP2021).
<!--- -------------------------------------------------------------------- --->

## Acknowledgements

This work has been supported in part
by EPSRC via grant
[EP/R012288/1](https://gow.epsrc.ukri.org/NGBOViewGrant.aspx?GrantRef=EP/R012288/1) (under the [RISE](http://www.ukrise.org) programme).
