// hive_pkg.cpp
#pragma once

/////////////
// DEFINES //
/////////////

#ifdef MSWIN
	// MSWin key codes //
	// concatenated byte key codes (printable ASCII: 0x20 thru 0x7e)
	#define KBD_TAB			0x9
	#define KBD_ENT			0xd
	#define KBD_ESC			0x1b
	#define KBD_BKSP		0x8
	//
	#define KBD_UP			0x48e0
	#define KBD_DN			0x50e0
	#define KBD_RT			0x4de0
	#define KBD_LT			0x4be0
	#define KBD_PGUP		0x49e0
	#define KBD_PGDN		0x51e0
	#define KBD_HOME		0x47e0
	#define KBD_END			0x4fe0
	#define KBD_INS			0x52e0
	#define KBD_DEL			0x53e0
	#define KBD_F1			0x3b00
	#define KBD_F2			0x3c00
	#define KBD_F3			0x3d00
	#define KBD_F4			0x3e00
	#define KBD_F5			0x3f00
	#define KBD_F6			0x4000
	#define KBD_F7			0x4100
	#define KBD_F8			0x4200
	#define KBD_F9			0x4300
	#define KBD_F10			0x4400
	#define KBD_F11			0x8500
	#define KBD_F12			0x8600
	//
	#define KBD_CTRL_UP		0x8de0
	#define KBD_CTRL_DN		0x91e0
	#define KBD_CTRL_RT		0x74e0
	#define KBD_CTRL_LT		0x73e0
	#define KBD_CTRL_PGUP	0x86e0
	#define KBD_CTRL_PGDN	0x76e0
	#define KBD_CTRL_HOME	0x77e0
	#define KBD_CTRL_END	0x75e0
	#define KBD_CTRL_DEL	0x93e0
	#define KBD_CTRL_F1		0x5e00
	#define KBD_CTRL_F2		0x5f00
	#define KBD_CTRL_F3		0x6000
	#define KBD_CTRL_F4		0x6100
	#define KBD_CTRL_F5		0x6200
	#define KBD_CTRL_F6		0x6300
	#define KBD_CTRL_F7		0x6400
	#define KBD_CTRL_F8		0x6500
	#define KBD_CTRL_F9		0x6600
	#define KBD_CTRL_F10	0x6700
	#define KBD_CTRL_F11	0x8900
	#define KBD_CTRL_F12	0x8a00
	//
	#define KBD_SHFT_F1		0x5400
	#define KBD_SHFT_F2		0x5500
	#define KBD_SHFT_F3		0x5600
	#define KBD_SHFT_F4		0x5700
	#define KBD_SHFT_F5		0x5800
	#define KBD_SHFT_F6		0x5900
	#define KBD_SHFT_F7		0x5a00
	#define KBD_SHFT_F8		0x5b00
	#define KBD_SHFT_F9		0x5c00
	#define KBD_SHFT_F10	0x5d00
	#define KBD_SHFT_F11	0x8700
	#define KBD_SHFT_F12	0x8800
	//
	#define KBD_ALT_UP		0x9800
	#define KBD_ALT_DN		0xa000
	#define KBD_ALT_RT		0x9d00
	#define KBD_ALT_LT		0x9b00
	#define KBD_ALT_PGUP	0x9900
	#define KBD_ALT_PGDN	0xa100
	#define KBD_ALT_HOME	0x9700
	#define KBD_ALT_END		0x9f00
	#define KBD_ALT_INS		0xa200
	#define KBD_ALT_DEL		0xa300
#else
	// POSIX key codes //
	// concatenated byte key codes (printable ASCII: 0x20 thru 0x7e)
	#define KBD_TAB			0x9
	#define KBD_ENT			0xa
	#define KBD_ESC			0x1b
	#define KBD_BKSP		0x7f
	//
	#define KBD_UP			0x1b5b41
	#define KBD_DN			0x1b5b42
	#define KBD_RT			0x1b5b43
	#define KBD_LT			0x1b5b44
	#define KBD_PGUP		0x1b5b357e
	#define KBD_PGDN		0x1b5b367e
	#define KBD_HOME		0x1b5b48
	#define KBD_END			0x1b5b46
	#define KBD_INS			0x1b5b327e
	#define KBD_DEL			0x1b5b337e
	#define KBD_F1			0x1b4f50
	#define KBD_F2			0x1b4f51
	#define KBD_F3			0x1b4f52
	#define KBD_F4			0x1b4f53
	#define KBD_F5			0x1b5b31357e
	#define KBD_F6			0x1b5b31377e
	#define KBD_F7			0x1b5b31387e
	#define KBD_F8			0x1b5b31397e
	#define KBD_F9			0x1b5b32307e
	#define KBD_F12			0x1b5b32347e
	//
	#define KBD_CTRL_UP		0x1b5b313b3541
	#define KBD_CTRL_DN		0x1b5b313b3542
	#define KBD_CTRL_RT		0x1b5b313b3543
	#define KBD_CTRL_LT		0x1b5b313b3544
	#define KBD_CTRL_PGUP	0x1b5b353b357e
	#define KBD_CTRL_PGDN	0x1b5b363b357e
	#define KBD_CTRL_HOME	0x1b5b313b3548
	#define KBD_CTRL_END	0x1b5b313b3546
	#define KBD_CTRL_DEL	0x1b5b333b357e
	#define KBD_CTRL_F1		0x1b5b313b3550
	#define KBD_CTRL_F2		0x1b5b313b3551
	#define KBD_CTRL_F3		0x1b5b313b3552
	#define KBD_CTRL_F4		0x1b5b313b3553
	#define KBD_CTRL_F5		0x1b5b31353b357e
	#define KBD_CTRL_F6		0x1b5b31373b357e
	#define KBD_CTRL_F7		0x1b5b31383b357e
	#define KBD_CTRL_F8		0x1b5b31393b357e
	#define KBD_CTRL_F9		0x1b5b32303b357e
	#define KBD_CTRL_F12	0x1b5b32343b357e
	//
	#define KBD_SHFT_F1		0x1b5b313b3250
	#define KBD_SHFT_F2		0x1b5b313b3251
	#define KBD_SHFT_F3		0x1b5b313b3252
	#define KBD_SHFT_F4		0x1b5b313b3253
	#define KBD_SHFT_F5		0x1b5b31353b327e
	#define KBD_SHFT_F6		0x1b5b31373b327e
	#define KBD_SHFT_F7		0x1b5b31383b327e
	#define KBD_SHFT_F8		0x1b5b31393b327e
	#define KBD_SHFT_F9		0x1b5b32303b327e
	#define KBD_SHFT_F12	0x1b5b32343b327e
	//
	#define KBD_ALT_UP		0x1b5b313b3341
	#define KBD_ALT_DN		0x1b5b313b3342
	#define KBD_ALT_RT		0x1b5b313b3343
	#define KBD_ALT_LT		0x1b5b313b3344
	#define KBD_ALT_PGUP	0x1b5b353b337e
	#define KBD_ALT_PGDN	0x1b5b363b337e
	#define KBD_ALT_HOME	0x1b5b313b3348
	#define KBD_ALT_END		0x1b5b313b3346
	#define KBD_ALT_INS		0x1b5b323b337e
	#define KBD_ALT_DEL		0x1b5b333b337e
#endif


// returns ceiling(log2(n)) of input
// (why isn't this in std?)
uint32_t clog2(uint32_t a_i) {
	uint32_t clog2a = 0;
	uint32_t i = a_i;
	while(i > 1) {
		i = (i + 1) >> 1;
		clog2a++;
	}
	return(clog2a);
};


///////////////
// CONSTANTS //
///////////////

// console size
const short CON_W				= 128;
const short CON_H				= 52;

// VERSION
const uint32_t VER				= 0x1304; // core version

// basic params
// DO NOT CHANGE!
const int32_t THREADS				= 8; // number of threads
const int32_t STACKS				= 8; // number of stacks
const int32_t ALU_W					= 32; // ALU width
const int32_t RBUS_ADDR_W			= 4; // rbus address width
const int32_t ALU_F_W					= 4; // alu flags width

// ok to change
const int32_t PC_W				= 14; // program counter width
const int32_t MEM_ADDR_W		= 14; // main memory address width
const int32_t STK_PTR_W			= 5; // stack pointer width
const bool STK_PROT_POP			= true; // stack error protection
const bool STK_PROT_PSH			= true; // stack error protection
const int32_t FIFO_ADDR_W		= 10; // fifo memory address width
const uint32_t BOOT_ADDR		= 0x3F00;  // boot loader mem start addr (256 bytes)
const uint32_t SPI_OFFS			= 0x10000;  // spi mem start addr
const int32_t LBL_PASS_MAX		= 100; 

// derivations of basic params
const int32_t THD_W					= clog2(THREADS);
const int32_t THD_MSK				= THREADS - 1;
const uint64_t PC_DEPTH			= uint64_t(1) << PC_W;
const uint32_t PC_MSK			= PC_DEPTH - 1;
const uint64_t MEM_DEPTH		= uint64_t(1) << MEM_ADDR_W;
const uint32_t MEM_ADDR_MSK		= MEM_DEPTH - 1;
const uint64_t RBUS_DEPTH		= uint64_t(1) << RBUS_ADDR_W;
const uint32_t RBUS_MSK			= RBUS_DEPTH - 1;
const int32_t HLF_W					= ALU_W >> 1;
const int32_t BYT_W					= ALU_W >> 2;
const int32_t SHL_W					= clog2(ALU_W);
const uint32_t STK_DEPTH		= 1 << STK_PTR_W; 
const uint32_t STK_ADDR_MSK		= STK_DEPTH - 1; 
const uint32_t FIFO_DEPTH		= 1 << FIFO_ADDR_W;
const uint32_t FIFO_ADDR_MSK	= FIFO_DEPTH - 1;
const uint32_t SPI_LENGTH		= BOOT_ADDR + 8;  // BOOT_ADDR + length + crc

// clr & irq addresses
const uint32_t CLT_BASE			= 0x0;
const uint32_t CLT_SPAN			= 0x2;
const uint32_t IRQ_BASE			= 0x20;
const uint32_t IRQ_SPAN			= 0x2;

// register set addresses
const uint8_t RBUS_VECT			= 0x0;
const uint8_t RBUS_TIME			= 0x1;
const uint8_t RBUS_ERROR		= 0x2;
const uint8_t RBUS_IO			= 0x3;
const uint8_t RBUS_UART_TX		= 0x4;
const uint8_t RBUS_UART_RX		= 0x5;
const uint8_t RBUS_SPI			= 0x6;
const uint8_t RBUS_SPDIF_L		= 0x8;
const uint8_t RBUS_SPDIF_R		= 0x9;
const uint8_t RBUS_PITCH		= 0xA;
const uint8_t RBUS_VOLUME		= 0xB;
const uint8_t RBUS_TUNER		= 0xC;
const uint8_t RBUS_LCD			= 0xD;
const uint8_t RBUS_ENC			= 0xE;
const string REG_NAMES[] = { "VECT", "TIME", "ERROR", "IO", "U_TX", "U_RX", "SPI", " ", "SPD_L", "SPD_R", "PTCH", "VOL", "TUNER", "LCD", "ENC" };

// reset / cfg values
const uint32_t ID_RST[THREADS] = {1, 0, 7, 6, 5, 4, 3, 2};

// lg alias values
const uint8_t LG_CPY			= 0x0;
const uint8_t LG_NSB			= 0x1;
const uint8_t LG_LIM			= 0x2;
const uint8_t LG_SAT			= 0x3;
const uint8_t LG_FLP			= 0x4;
const uint8_t LG_SWP			= 0x5;
const uint8_t LG_NOT			= 0x6;
const uint8_t LG_BRX			= 0x8;
const uint8_t LG_SGN			= 0x9;
const uint8_t LG_LZC			= 0xA;
const uint8_t LG_AND			= 0xC;
const uint8_t LG_ORR			= 0xD;
const uint8_t LG_XOR			= 0xE;

// test alias values
const uint8_t TST_Z				= 0x0;
const uint8_t TST_NZ			= 0x1;
const uint8_t TST_LZ			= 0x2;
const uint8_t TST_NLZ			= 0x3;
const uint8_t TST_O				= 0x4;
const uint8_t TST_NO			= 0x5;
const uint8_t TST_E				= 0x6;
const uint8_t TST_NE			= 0x7;
const uint8_t TST_LS			= 0x8;
const uint8_t TST_NLS			= 0x9;
const uint8_t TST_LU			= 0xA;
const uint8_t TST_NLU			= 0xB;


// opcode byte values
const uint8_t OP_NOP			= 0x00;
const uint8_t OP_HLT			= 0x01;
//
const uint8_t OP_POP			= 0x04;
const uint8_t OP_CLS			= 0x05;
//
const uint8_t OP_JMP_8			= 0x08;
const uint8_t OP_JMP_16			= 0x09;
const uint8_t OP_JMP_24			= 0x0A;
//
const uint8_t OP_ADD_XU			= 0x20;
const uint8_t OP_ADD_XUS		= 0x21;
const uint8_t OP_ADD_XSU		= 0x22;
const uint8_t OP_ADD_XS			= 0x23;
const uint8_t OP_SUB_XU			= 0x24;
const uint8_t OP_SUB_XUS		= 0x25;
const uint8_t OP_SUB_XSU		= 0x26;
const uint8_t OP_SUB_XS			= 0x27;
const uint8_t OP_SBR_XU			= 0x28;
const uint8_t OP_SBR_XUS		= 0x29;
const uint8_t OP_SBR_XSU		= 0x2A;
const uint8_t OP_SBR_XS			= 0x2B;
const uint8_t OP_MUL_XU			= 0x2C;
const uint8_t OP_MUL_XUS		= 0x2D;
const uint8_t OP_MUL_XSU		= 0x2E;
const uint8_t OP_MUL_XS			= 0x2F;
//
const uint8_t OP_CPY			= 0x30 + LG_CPY;
const uint8_t OP_NSB			= 0x30 + LG_NSB;
const uint8_t OP_LIM			= 0x30 + LG_LIM;
const uint8_t OP_SAT			= 0x30 + LG_SAT;
const uint8_t OP_FLP			= 0x30 + LG_FLP;
const uint8_t OP_SWP			= 0x30 + LG_SWP;
const uint8_t OP_NOT			= 0x30 + LG_NOT;
const uint8_t OP_BRX			= 0x30 + LG_BRX;
const uint8_t OP_SGN			= 0x30 + LG_SGN;
const uint8_t OP_LZC			= 0x30 + LG_LZC;
//
const uint8_t OP_JMP_Z			= 0x40 + TST_Z;
const uint8_t OP_JMP_NZ			= 0x40 + TST_NZ;
const uint8_t OP_JMP_LZ			= 0x40 + TST_LZ;
const uint8_t OP_JMP_NLZ		= 0x40 + TST_NLZ;
const uint8_t OP_JSB			= 0x48;
const uint8_t OP_JMP			= 0x49;
const uint8_t OP_GSB			= 0x4A;
const uint8_t OP_GTO			= 0x4B;
const uint8_t OP_IRT			= 0x4C;
const uint8_t OP_PCR			= 0x4E;
//
const uint8_t OP_MEM_R			= 0x50;
const uint8_t OP_MEM_W			= 0x51;
const uint8_t OP_MEM_WH			= 0x52;
const uint8_t OP_MEM_WB			= 0x53;
const uint8_t OP_MEM_RHS		= 0x54;
const uint8_t OP_MEM_RHU		= 0x55;
const uint8_t OP_MEM_RBS		= 0x56;
const uint8_t OP_MEM_RBU		= 0x57;
const uint8_t OP_LIT			= 0x58;
const uint8_t OP_LIT_HS			= 0x5C;
const uint8_t OP_LIT_HU			= 0x5D;
const uint8_t OP_LIT_BS			= 0x5E;
const uint8_t OP_LIT_BU			= 0x5F;
//
const uint8_t OP_ADD_U			= 0x60;
const uint8_t OP_ADD_US			= 0x61;
const uint8_t OP_ADD_SU			= 0x62;
const uint8_t OP_ADD_S			= 0x63;
const uint8_t OP_SUB_U			= 0x64;
const uint8_t OP_SUB_US			= 0x65;
const uint8_t OP_SUB_SU			= 0x66;
const uint8_t OP_SUB_S			= 0x67;
const uint8_t OP_SBR_U			= 0x68;
const uint8_t OP_SBR_US			= 0x69;
const uint8_t OP_SBR_SU			= 0x6A;
const uint8_t OP_SBR_S			= 0x6B;
const uint8_t OP_MUL_U			= 0x6C;
const uint8_t OP_MUL_US			= 0x6D;
const uint8_t OP_MUL_SU			= 0x6E;
const uint8_t OP_MUL_S			= 0x6F;
//
const uint8_t OP_SHL_U			= 0x70;
const uint8_t OP_SHL_S			= 0x71;
const uint8_t OP_ROL			= 0x72;
const uint8_t OP_POW			= 0x73;
const uint8_t OP_REG_R			= 0x78;
const uint8_t OP_REG_W			= 0x79;
const uint8_t OP_AND			= 0x70 + LG_AND;
const uint8_t OP_ORR			= 0x70 + LG_ORR;
const uint8_t OP_XOR			= 0x70 + LG_XOR;
//
const uint8_t OP_JMP_8Z			= 0x80 + TST_Z;
const uint8_t OP_JMP_8NZ		= 0x80 + TST_NZ;
const uint8_t OP_JMP_8LZ		= 0x80 + TST_LZ;
const uint8_t OP_JMP_8NLZ		= 0x80 + TST_NLZ;
const uint8_t OP_JMP_8O			= 0x80 + TST_O;
const uint8_t OP_JMP_8NO		= 0x80 + TST_NO;
const uint8_t OP_JMP_8E			= 0x80 + TST_E;
const uint8_t OP_JMP_8NE		= 0x80 + TST_NE;
const uint8_t OP_JMP_8LS		= 0x80 + TST_LS;
const uint8_t OP_JMP_8NLS		= 0x80 + TST_NLS;
const uint8_t OP_JMP_8LU		= 0x80 + TST_LU;
const uint8_t OP_JMP_8NLU		= 0x80 + TST_NLU;
const uint8_t OP_JSB_8			= 0x8C;
//
const uint8_t OP_MEM_8R			= 0x90;
const uint8_t OP_MEM_8W			= 0x91;
const uint8_t OP_MEM_8WH		= 0x92;
const uint8_t OP_MEM_8WB		= 0x93;
const uint8_t OP_MEM_8RHS		= 0x94;
const uint8_t OP_MEM_8RHU		= 0x95;
const uint8_t OP_MEM_8RBS		= 0x96;
const uint8_t OP_MEM_8RBU		= 0x97;
const uint8_t OP_MEM_I8R		= 0x98;
const uint8_t OP_MEM_I8W		= 0x99;
const uint8_t OP_MEM_I8WH		= 0x9A;
const uint8_t OP_MEM_I8WB		= 0x9B;
const uint8_t OP_MEM_I8RHS		= 0x9C;
const uint8_t OP_MEM_I8RHU		= 0x9D;
const uint8_t OP_MEM_I8RBS		= 0x9E;
const uint8_t OP_MEM_I8RBU		= 0x9F;
//
const uint8_t OP_ADD_8U			= 0xA0;
const uint8_t OP_ADD_8US		= 0xA1;
const uint8_t OP_ADD_8SU		= 0xA2;
const uint8_t OP_ADD_8S			= 0xA3;
const uint8_t OP_SUB_8U			= 0xA4;
const uint8_t OP_SUB_8US		= 0xA5;
const uint8_t OP_SUB_8SU		= 0xA6;
const uint8_t OP_SUB_8S			= 0xA7;
const uint8_t OP_SBR_8U			= 0xA8;
const uint8_t OP_SBR_8US		= 0xA9;
const uint8_t OP_SBR_8SU		= 0xAA;
const uint8_t OP_SBR_8S			= 0xAB;
const uint8_t OP_MUL_8U			= 0xAC;
const uint8_t OP_MUL_8US		= 0xAD;
const uint8_t OP_MUL_8SU		= 0xAE;
const uint8_t OP_MUL_8S			= 0xAF;
//
const uint8_t OP_SHL_8U			= 0xB0;
const uint8_t OP_SHL_8S			= 0xB1;
const uint8_t OP_ROL_8			= 0xB2;
const uint8_t OP_POW_8			= 0xB3;
const uint8_t OP_REG_8R			= 0xB8;
const uint8_t OP_REG_8W			= 0xB9;
const uint8_t OP_AND_8			= 0xB0 + LG_AND;
const uint8_t OP_ORR_8			= 0xB0 + LG_ORR;
const uint8_t OP_XOR_8			= 0xB0 + LG_XOR;
//
const uint8_t OP_JMP_16Z		= 0xC0 + TST_Z;
const uint8_t OP_JMP_16NZ		= 0xC0 + TST_NZ;
const uint8_t OP_JMP_16LZ		= 0xC0 + TST_LZ;
const uint8_t OP_JMP_16NLZ		= 0xC0 + TST_NLZ;
const uint8_t OP_JMP_16O		= 0xC0 + TST_O;
const uint8_t OP_JMP_16NO		= 0xC0 + TST_NO;
const uint8_t OP_JMP_16E		= 0xC0 + TST_E;
const uint8_t OP_JMP_16NE		= 0xC0 + TST_NE;
const uint8_t OP_JMP_16LS		= 0xC0 + TST_LS;
const uint8_t OP_JMP_16NLS		= 0xC0 + TST_NLS;
const uint8_t OP_JMP_16LU		= 0xC0 + TST_LU;
const uint8_t OP_JMP_16NLU		= 0xC0 + TST_NLU;
const uint8_t OP_JSB_16			= 0xCC;
//
const uint8_t OP_MEM_16R		= 0xD0;
const uint8_t OP_MEM_16W		= 0xD1;
const uint8_t OP_MEM_16WH		= 0xD2;
const uint8_t OP_MEM_16WB		= 0xD3;
const uint8_t OP_MEM_16RHS		= 0xD4;
const uint8_t OP_MEM_16RHU		= 0xD5;
const uint8_t OP_MEM_16RBS		= 0xD6;
const uint8_t OP_MEM_16RBU		= 0xD7;
const uint8_t OP_MEM_I16R		= 0xD8;
const uint8_t OP_MEM_I16W		= 0xD9;
const uint8_t OP_MEM_I16WH		= 0xDA;
const uint8_t OP_MEM_I16WB		= 0xDB;
const uint8_t OP_MEM_I16RHS		= 0xDC;
const uint8_t OP_MEM_I16RHU		= 0xDD;
const uint8_t OP_MEM_I16RBS		= 0xDE;
const uint8_t OP_MEM_I16RBU		= 0xDF;
//
const uint8_t OP_ADD_16U		= 0xE0;
const uint8_t OP_ADD_16US		= 0xE1;
const uint8_t OP_ADD_16SU		= 0xE2;
const uint8_t OP_ADD_16S		= 0xE3;
const uint8_t OP_SUB_16U		= 0xE4;
const uint8_t OP_SUB_16US		= 0xE5;
const uint8_t OP_SUB_16SU		= 0xE6;
const uint8_t OP_SUB_16S		= 0xE7;
const uint8_t OP_SBR_16U		= 0xE8;
const uint8_t OP_SBR_16US		= 0xE9;
const uint8_t OP_SBR_16SU		= 0xEA;
const uint8_t OP_SBR_16S		= 0xEB;
const uint8_t OP_MUL_16U		= 0xEC;
const uint8_t OP_MUL_16US		= 0xED;
const uint8_t OP_MUL_16SU		= 0xEE;
const uint8_t OP_MUL_16S		= 0xEF;
//
const uint8_t OP_AND_16			= 0xF0 + LG_AND;
const uint8_t OP_ORR_16			= 0xF0 + LG_ORR;
const uint8_t OP_XOR_16			= 0xF0 + LG_XOR;


/////////////
// STRUCTS //
/////////////

// ALU type, data w/ extended results flags
struct alu_t {
	int32_t val;
   	bitset<ALU_F_W> flg;
};
  

//////////////////////
// HELPER FUNCTIONS //
//////////////////////

// returns unsigned modulo of input for a given bit width
uint32_t bmodu(uint32_t a_i, uint32_t bits_i) {
	return((a_i << (32 - bits_i)) >> (32 - bits_i));
};

// returns signed modulo of input for a given bit width
int32_t bmods(int32_t a_i, uint32_t bits_i) {
	return((a_i << (32 - bits_i)) >> (32 - bits_i));
};

// test for valid general whitespace char - tab, newline, vertical tab, form feed, carriage return, space
bool ch_is_white(int32_t ch_i) {
	return ((ch_i == '\t') || (ch_i == '\n') || (ch_i == '\r') || (ch_i == '\v') || (ch_i == '\f') || (ch_i == ' '));
}

// test for valid general non-whitespace char - surprisingly contiguous!
bool ch_is_nonwhite(int32_t ch_i) {
	return ((ch_i >= '!') && (ch_i <= '~'));
}

// test for valid white or non-white char
bool ch_is_print(int32_t ch_i) {
	return (ch_is_white(ch_i) || ch_is_nonwhite(ch_i));
}

// test for upper case alpha char
bool ch_is_upper(int32_t ch_i) {
	return ((ch_i >= 'A') && (ch_i <= 'Z'));
}

// test for lower case alpha char
bool ch_is_lower(int32_t ch_i) {
	return ((ch_i >= 'a') && (ch_i <= 'z'));
}

// change alpha char to upper case
int32_t ch_to_upper(int32_t ch_i) {
	return (ch_is_lower(ch_i) ? ch_i - 32 : ch_i);
}

// change alpha char to lower case
int32_t ch_to_lower(int32_t ch_i) {
	return (ch_is_upper(ch_i) ? ch_i + 32 : ch_i);
}

// test for A-Z, a-z char, underscore
bool ch_is_alpha(int32_t ch_i) {
	return (ch_is_upper(ch_i) || ch_is_lower(ch_i) || (ch_i == '_'));
}

// test for 0-9
bool ch_is_digit(int32_t ch_i) {
	return ((ch_i >= '0') && (ch_i <= '9'));
}

// test for 0,1 bin char
bool ch_is_bin(int32_t ch_i) {
	return ((ch_i == '0') || (ch_i == '1'));
}

// test for 0-7 oct char
bool ch_is_oct(int32_t ch_i) {
	return ((ch_i >= '0') && (ch_i <= '7'));
}

// test for 0-9, A-F, a-f hex char
bool ch_is_hex(int32_t ch_i) {
	return (ch_is_digit(ch_i) || ((ch_to_upper(ch_i) >= 'A') && (ch_to_upper(ch_i) <= 'F')));
}

// convert ch to hex int value
int32_t ch_to_hex(int32_t ch_i) {
	if (ch_is_digit(ch_i)) { return (ch_i - '0'); }
	else if (ch_is_lower(ch_i)) { return (ch_i + 0xA - 'a'); }
	else if (ch_is_upper(ch_i)) { return (ch_i + 0xA - 'A'); }
	else { return (0); }
}

// convert hex int value to ch
char hex_to_ch(int32_t int_i) {
	if (int_i >= 0 && int_i <= 9) { return (int_i + '0'); }
	else if (int_i >= 0xa && int_i <= 0xf) { return (int_i - 0xa + 'a'); }
	else { return (' '); }
}

// change string alpha to lower case
string str_to_lower(const string& str_i) {
	string str_o = "";
	for (uint32_t i = 0; i < str_i.length(); i++) { str_o += ch_to_lower(str_i[i]); }
	return (str_o);
}

// trim whitespace at end of string
string str_trim(const string& str_i) {
	string str = str_i;
    while (ch_is_white(str.front())) { str.erase(str.begin()); }
    while (ch_is_white(str.back())) { str.pop_back(); }
   	return (str);
}

// return radix base
int32_t radix_to_int(const string& radix_i) {
	if (radix_i == "BIN") { return (2); }
	else if (radix_i == "OCT") { return (8); }
	else if ((radix_i == "UNS") || (radix_i == "INT")) { return (10); }
	else if (radix_i == "HEX") { return (16); }
	else return (1);
}

// test string for compliance to int with given radix
// "AUTO" picks between "HEX", "BIN", and "INT" based on input "0x" and "0b"
bool str_is_int(const string& str_i, const string& radix_i) {
	string sub = "";  // substring
	int32_t sub_len = 0;  // substring length
	int32_t str_len = str_i.length();  // get input string length
	string radix = radix_i;  // follow input radix
	bool neg_f = false;  // negative sign flag
	int32_t i = 0;  // index
	// check for pos / neg sign
	sub = str_i.substr(i, 1);
	sub_len = sub.length();
	if (sub_len) {
		if (sub == "+") { i++; } 
		else if (sub == "-") { neg_f = true; i++; } 
	}
	// check for "0x"
	sub = str_i.substr(i, 2);
	sub_len = sub.length();
	if (((radix_i == "HEX") || (radix_i == "AUTO")) && ((sub_len == 2) && (str_to_lower(sub) == "0x"))) { radix = "HEX"; i += 2; }  
	// check for "0b"
	else if (((radix_i == "BIN") || (radix_i == "AUTO")) && ((sub_len == 2) && (str_to_lower(sub) == "0b"))) { radix = "BIN"; i += 2; }  
	// neither and AUTO
	else if (radix_i == "AUTO") { radix = "INT"; }
	// string must have body info
	bool test_f = (i < str_len) ? true : false;
	// radix "UNS" cannot be negative
	test_f &= !(neg_f && (radix == "UNS"));
	// test string body to radix
	while (i < str_len) {
		if (radix == "BIN") { test_f &= ch_is_bin(str_i[i]); }
		else if (radix == "OCT") { test_f &= ch_is_oct(str_i[i]); }
		else if ((radix == "UNS") || (radix == "INT")) { test_f &= isdigit(str_i[i]); }
		else if (radix == "HEX") { test_f &= ch_is_hex(str_i[i]); }
		i++;
	}
	return (test_f);
}

// convert string to int given radix
// "AUTO" picks between "HEX", "BIN", and "INT" based on input "0x" and "0b"
// no radix checks are performed on the remaining content!  use str_is_int() to check
int32_t str_to_int(const string& str_i, const string& radix_i) {
	string sub = "";  // substring
	int32_t sub_len = 0;  // substring length
	int32_t str_len = str_i.length();  // get input string length
	string radix = radix_i;  // follow input radix
	bool neg_f = false;  // negative sign flag
	int32_t i = 0;  // loop index

	// check for pos / neg sign
	sub = str_i.substr(i, 1);
	sub_len = sub.length();
	if (sub_len) {
		if (sub == "+") { i++; } 
		else if (sub == "-") { neg_f = true; i++; } 
	}
	// check for "0x"
	sub = str_i.substr(i, 2);
	sub_len = sub.length();
	if (((radix_i == "HEX") || (radix_i == "AUTO")) && ((sub_len == 2) && (str_to_lower(sub) == "0x"))) { radix = "HEX"; i += 2; }  
	// check for "0b"
	else if (((radix_i == "BIN") || (radix_i == "AUTO")) && ((sub_len == 2) && (str_to_lower(sub) == "0b"))) { radix = "BIN"; i += 2; }  
	// neither and AUTO
	else if (radix_i == "AUTO") { radix = "INT"; }
	// convert string body to int
	int32_t base = radix_to_int(radix);
	int32_t result = 0;
	while (i < str_len) {
		result *= base;
		result += ch_to_hex(str_i[i]);
		i++;
	}
	return (neg_f ? -result : result);
}

// convert int32_t to string with given radix and bit length
// radix "BIN" "OCT" "UNS" and "HEX" are unsigned, "INT" is signed
string int_to_str(int32_t int_i, const string& radix_i, uint32_t bits_i, bool showbase_i) {
	int32_t base = radix_to_int(radix_i);  // get base number
	int32_t sgn_i = bmods(int_i, bits_i);
	uint32_t uns_i = bmodu(int_i, bits_i);
	bool neg_f = (sgn_i < 0) ? true : false;  // check sign
	if (radix_i == "INT") { uns_i = (neg_f) ? -sgn_i : sgn_i; }
	string str_o = "";
	while (uns_i > 0) {
		str_o = char(hex_to_ch(uns_i % base)) + str_o;
		uns_i /= base;
	}
	if (str_o.empty()) { str_o = "0"; }
	if ((radix_i == "BIN") && showbase_i && sgn_i) { str_o = "0b" + str_o; }
	if ((radix_i == "HEX") && showbase_i && sgn_i) { str_o = "0x" + str_o; }
	if ((radix_i == "INT") && neg_f) { str_o = '-' + str_o; }
	return (str_o);
}

// oc pa
bool oc_pa (uint16_t oc_i) { return (oc_i & 0x800); }

// oc pb
bool oc_pb (uint16_t oc_i) { return (oc_i & 0x8000); }

// oc sa
int32_t oc_sa (uint16_t oc_i) { return ((oc_i >> 8) & 0x7); }

// oc sb
int32_t oc_sb (uint16_t oc_i) { return ((oc_i >> 12) & 0x7); }

// do CRC32 operation
// I/O is 32 bit chunks 
// 1. Logical NOT first input chunk and call routine to produce first output chunk. 
// 2. XOR next input chunk with previous output chunk and call routine to produce next output chunk, etc. 
// 3. Logical NOT final output chunk to obtain CRC32. 
// 4. Call routine one last time (with CRC32 as input) to get final output chunk. 
// Passing output message through steps 1 & 2 should produce final output chunk 0xDEBB20E3 
uint32_t crc_32(uint32_t data_i) {
	uint32_t data_o = data_i;  // shift reg
	uint32_t poly = 0x6DB88320;  // crc32 polynomial
	for (int32_t i = 0; i < 32; i++) {
		data_o = (data_o << 31) | (data_o >> 1);  // rotate right 1
		if (int32_t(data_o) < 0) { data_o ^= poly; }  // xor w/ poly if MSb set
	}
	return(data_o);
}

// sleep for microseconds
void us_sleep(uint32_t us_i) {
	this_thread::sleep_for(chrono::microseconds(us_i));
	return;
}
