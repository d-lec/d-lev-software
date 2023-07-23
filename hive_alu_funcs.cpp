// hive_alu_funcs.cpp
#pragma once

// basic double-wide ops
uint64_t add_uu(uint32_t a_i, uint32_t b_i) { return(uint64_t(a_i) + uint64_t(b_i)); }
uint64_t add_us(uint32_t a_i,  int32_t b_i) { return(uint64_t(a_i) +  int64_t(b_i)); }
 int64_t add_su( int32_t a_i, uint32_t b_i) { return( int64_t(a_i) + uint64_t(b_i)); }
 int64_t add_ss( int32_t a_i,  int32_t b_i) { return( int64_t(a_i) +  int64_t(b_i)); }
//
uint64_t sub_uu(uint32_t a_i, uint32_t b_i) { return(uint64_t(a_i) - uint64_t(b_i)); }
uint64_t sub_us(uint32_t a_i,  int32_t b_i) { return(uint64_t(a_i) -  int64_t(b_i)); }
 int64_t sub_su( int32_t a_i, uint32_t b_i) { return( int64_t(a_i) - uint64_t(b_i)); }
 int64_t sub_ss( int32_t a_i,  int32_t b_i) { return( int64_t(a_i) -  int64_t(b_i)); }
//
uint64_t sbr_uu(uint32_t a_i, uint32_t b_i) { return(uint64_t(b_i) - uint64_t(a_i)); }
uint64_t sbr_us( int32_t a_i, uint32_t b_i) { return(uint64_t(b_i) -  int64_t(a_i)); }
 int64_t sbr_su(uint32_t a_i,  int32_t b_i) { return( int64_t(b_i) - uint64_t(a_i)); }
 int64_t sbr_ss( int32_t a_i,  int32_t b_i) { return( int64_t(b_i) -  int64_t(a_i)); }
//
uint64_t mul_uu(uint32_t a_i, uint32_t b_i) { return(uint64_t(a_i) * uint64_t(b_i)); }
uint64_t mul_us(uint32_t a_i,  int32_t b_i) { return(uint64_t(a_i) *  int64_t(b_i)); }
 int64_t mul_su( int32_t a_i, uint32_t b_i) { return( int64_t(a_i) * uint64_t(b_i)); }
 int64_t mul_ss( int32_t a_i,  int32_t b_i) { return( int64_t(a_i) *  int64_t(b_i)); }
// 
uint64_t shl_u(uint32_t a_i, int32_t b_i) { return(uint64_t(a_i) << bmodu(b_i, SHL_W)); }
 int64_t shl_s( int32_t a_i, int32_t b_i) { return( int64_t(a_i) << bmodu(b_i, SHL_W)); }

////////////////
// arithmetic //
////////////////
alu_t alu_add(alu_t a_i, alu_t b_i, bool as_i, bool bs_i, bool x_i) { 
	uint64_t res;
	alu_t ret;
	if      (!as_i && !bs_i) { res = add_uu(a_i.val, b_i.val); }
	else if (!as_i &&  bs_i) { res = add_us(a_i.val, b_i.val); }
	else if ( as_i && !bs_i) { res = add_su(a_i.val, b_i.val); }
	else                     { res = add_ss(a_i.val, b_i.val); }
	ret.flg[3] = (res >> 33) & 1;  // res[33] (sgn)
	ret.flg[2] = (res >> 32) & 1;  // res[32]
	ret.flg[1] = (res >> 32) & 1;  // res[32]
	ret.flg[0] = (res >> 31) & 1;  // res[31] (msb)
	ret.val = (x_i) ? (res >> ALU_W) : res;
	return(ret); 
}

alu_t alu_sub(alu_t a_i, alu_t b_i, bool as_i, bool bs_i, bool x_i) { 
	uint64_t res;
	alu_t ret;
	if      (!as_i && !bs_i) { res = sub_uu(a_i.val, b_i.val); }
	else if (!as_i &&  bs_i) { res = sub_us(a_i.val, b_i.val); }
	else if ( as_i && !bs_i) { res = sub_su(a_i.val, b_i.val); }
	else                     { res = sub_ss(a_i.val, b_i.val); }
	ret.flg[3] = (res >> 33) & 1;  // res[33] (sgn)
	ret.flg[2] = (res >> 32) & 1;  // res[32]
	ret.flg[1] = (res >> 32) & 1;  // res[32]
	ret.flg[0] = (res >> 31) & 1;  // res[31] (msb)
	ret.val = (x_i) ? (res >> ALU_W) : res;
	return(ret); 
}

alu_t alu_sbr(alu_t a_i, alu_t b_i, bool as_i, bool bs_i, bool x_i) { 
	uint64_t res;
	alu_t ret;
	if      (!as_i && !bs_i) { res = sbr_uu(a_i.val, b_i.val); }
	else if (!as_i &&  bs_i) { res = sbr_us(a_i.val, b_i.val); }
	else if ( as_i && !bs_i) { res = sbr_su(a_i.val, b_i.val); }
	else                     { res = sbr_ss(a_i.val, b_i.val); }
	ret.flg[3] = (res >> 33) & 1;  // res[33] (sgn)
	ret.flg[2] = (res >> 32) & 1;  // res[32]
	ret.flg[1] = (res >> 32) & 1;  // res[32]
	ret.flg[0] = (res >> 31) & 1;  // res[31] (msb)
	ret.val = (x_i) ? (res >> ALU_W) : res;
	return(ret); 
}

alu_t alu_mul(alu_t a_i, alu_t b_i, bool as_i, bool bs_i, bool x_i) { 
	uint64_t res;
	alu_t ret;
	if      (!as_i && !bs_i) { res = mul_uu(a_i.val, b_i.val); ret.flg[3] = 0; }
	else if (!as_i &&  bs_i) { res = mul_us(a_i.val, b_i.val); ret.flg[3] = (res >> 63) & 1; }
	else if ( as_i && !bs_i) { res = mul_su(a_i.val, b_i.val); ret.flg[3] = (res >> 63) & 1; }
	else                     { res = mul_ss(a_i.val, b_i.val); ret.flg[3] = (res >> 63) & 1; }
	ret.flg[2] = (int32_t(res >> 32) ==  0) ? 0 : 1;  // |res[63:32]
	ret.flg[1] = (int32_t(res >> 32) == -1) ? 1 : 0;  // &res[63:32]
	ret.flg[0] = (res >> 31) & 1;  // res[31] (msb)
	ret.val = (x_i) ? (res >> ALU_W) : res;
	return(ret); 
}

alu_t alu_lim(alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	bool olm = (~b_i.flg[3] & b_i.flg[2]);
	bool ulm = b_i.flg[3];
	ret.val = (olm) ? -1 : (ulm) ? 0 : b_i.val;
	return(ret); 
}

alu_t alu_sat(alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	uint32_t UFL = 1 << (ALU_W-1);
	bool ofl = ~b_i.flg[3] &  (b_i.flg[2] | b_i.flg[0]);
	bool ufl =  b_i.flg[3] & ~(b_i.flg[1] & b_i.flg[0]);
	ret.val = (ofl) ? ~UFL : (ufl) ? UFL : b_i.val;
	return(ret); 
}

///////////
// shift //
///////////
alu_t alu_shl(alu_t a_i, alu_t b_i, bool as_i) {
	uint64_t res;
	alu_t ret;
	ret.val = 0;
	ret.flg = 0;  // !FIXME! s/b result of mult!
	if ((b_i.val < -ALU_W) || (b_i.val >= ALU_W)) { return(ret); }
	if (as_i) { res = shl_s(a_i.val, b_i.val); }
	else      { res = shl_u(a_i.val, b_i.val); }
	ret.val = (b_i.val < 0) ? (res >> ALU_W) : res;
	return(ret);
}

alu_t alu_pow(alu_t b_i) { 
	alu_t ret;
	ret.val = 0;
	ret.flg = 0;  // !FIXME! s/b result of mult!
	if ((b_i.val < -ALU_W) || (b_i.val >= ALU_W)) { return(ret); }
	ret.val = shl_u(1, b_i.val);
	return(ret);
};

alu_t alu_rol(alu_t a_i, alu_t b_i) {
	alu_t ret;
	ret.flg = 0;  // !FIXME! s/b result of mult!
	uint64_t res = shl_u(a_i.val, b_i.val);
	ret.val = ((res >> ALU_W) | res);
	return(ret);
}

/////////////
// logical //
/////////////
alu_t alu_and(alu_t a_i, alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (a_i.val & b_i.val);
	return(ret);
}
	
alu_t alu_orr(alu_t a_i, alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (a_i.val | b_i.val);
	return(ret);
}

alu_t alu_xor(alu_t a_i, alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (a_i.val ^ b_i.val);
	return(ret);
}

alu_t alu_not(alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (~b_i.val);
	return(ret);
}

alu_t alu_brx(alu_t b_i) { 
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	int32_t b_xor = b_i.val;
	int32_t i = 1;
	while(i < ALU_W) {
		b_xor ^= (b_xor >> i);
		i *= 2;
	}
	ret.val = (b_xor & 1) ? -1 : 0;
	return(ret);
}

//////////
// misc //
//////////
alu_t alu_lzc(alu_t b_i) {  // leading zero count
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	uint32_t b = b_i.val;
	uint32_t lzc = ALU_W; 
	for(int32_t bit=0; bit<ALU_W; bit++) {
		if (b & 1) { lzc = ALU_W - 1 - bit; }
		b >>= 1;
	}
	ret.val = lzc;
	return(ret);
}

alu_t alu_flp(alu_t b_i) {  // flip end for end
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	uint32_t b = b_i.val;
	uint32_t flp = 0;
	for(int32_t bit=0; bit<ALU_W; bit++) {
		flp = (flp << 1) | (b & 1);
		b >>= 1;
	}
	ret.val = flp;
	return(ret);
}

alu_t alu_swp(alu_t b_i) {  // swap bytes
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	uint32_t b = b_i.val;
	ret.val = ((b >> 24) | ((b << 8) & 0x00FF0000) | ((b >> 8) & 0x0000FF00) | (b << 24));
	return(ret);
}

alu_t alu_nsb(alu_t b_i) {  // negate sign bit
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (b_i.val ^ 0x80000000); 
	return(ret);
};

alu_t alu_sgn(alu_t b_i) {  // sign value
	alu_t ret;
	ret.flg = b_i.flg;  // b flags pass thru
	ret.val = (b_i.val < 0) ? -1 : 1; 
	return(ret);
}

alu_t to_alu_t(uint32_t val_i) {  // convert value to alu_t
	alu_t ret;
	ret.flg = 0;  // clear flags
	ret.val = val_i;
	return(ret);
}


