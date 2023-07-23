// hive_main_mem.cpp
#pragma once

class main_mem {

public:

	// state and such
	uint8_t mem[MEM_DEPTH];  // byte addressed memory
	bool use_f[MEM_DEPTH];  // use flag
	bool lit_f[MEM_DEPTH];  // lit flag
	int32_t bytes[MEM_DEPTH];  // byte count of op/lit
	string file[MEM_DEPTH];  // file name
	uint32_t line[MEM_DEPTH];  // line number
	string eolc[MEM_DEPTH];  // end of line comments
	uint32_t crc;  // crc

	// HAL source code single line container
	struct src_t {  
	   	string file;
		uint32_t line = 0;
	   	string code;
	   	string eolc;
	};

	// read mem
	uint64_t rd(uint32_t addr_i, int32_t bytes_i) { 
		uint64_t data = 0;
		for (int32_t i = 0; i < bytes_i; i++) { 
			data |= uint64_t(mem[(addr_i + i) & MEM_ADDR_MSK]) << (i*8);
		}
		return (data);
	}
	
	// write mem
	void wr(uint32_t addr_i, uint32_t bytes_i, uint64_t data_i) { 
		for (uint64_t i = 0; i < bytes_i; i++) { 
			mem[(addr_i + i) & MEM_ADDR_MSK] = (data_i >> (i*8));
		}
	}
	
	// zero mem
	void z() { 
		for (uint32_t i = 0; i < MEM_DEPTH; i++) { 
			mem[i] = 0;
			use_f[i] = false;
			lit_f[i] = false;
			bytes[i] = 0;
			file[i] = "";
			line[i] = 0;
			eolc[i] = "";
		}
	}
		
	///////////////
	// HAL stuff //
	///////////////

	// test for sa, sb, pa, pb
	bool str_is_sab(const string& str_i) {
		return ((str_i.length() == 2) && ((tolower(str_i[0]) == 's') || (tolower(str_i[0]) == 'p')) && ch_is_oct(str_i[1]));
	}
	
	// test for pa, pb
	bool str_is_pop(const string& str_i) {
		return ((str_i.length() == 2) && (tolower(str_i[0]) == 'p') && ch_is_oct(str_i[1]));
	}
	
	// test for cls
	bool str_is_cls(const string& str_i) {
		return ((str_i.length() == 2) && (tolower(str_i[0]) == 'c') && ch_is_oct(str_i[1]));
	}
	
	// convert sa string to int
	uint8_t sa_to_int(const string& sa_i) {
		uint8_t sa_o = ch_to_hex(sa_i[1]);
		if (tolower(sa_i[0]) == 'p') { sa_o |= 8; }
		return (sa_o);
	}
	
	// convert sb string to int32_t
	uint8_t sb_to_int(const string& sb_i) {
		return (sa_to_int(sb_i) << 4);
	}
	
	// convert sa & sb strings to int
	uint8_t sab_to_int(const string& sa_i, const string& sb_i) {
		return (sa_to_int(sa_i) | sb_to_int(sb_i));
	}
	
	// convert pop, etc. string to int32_t
	uint8_t oh_to_int(const string& str_i) {
		return (1 << ch_to_hex(str_i[1]));
	}
	
	// convert oc to sa string
	string oc_sa_str(uint16_t oc_i) {
		string sa = "";
		if (oc_pa(oc_i)) { sa = "P"; } 
		else { sa = "s"; } 
		sa += int_to_str(oc_sa(oc_i), "OCT", 3, false);
		return (sa);
	}
	
	// convert oc to sb string
	string oc_sb_str(uint16_t oc_i) {
		string sb = "";
		if (oc_pb(oc_i)) { sb = "P"; } 
		else { sb = "s"; } 
		sb += int_to_str(oc_sb(oc_i), "OCT", 3, false);
		return (sb);
	}
	
	// convert oc to pa string if pop
	string oc_pa_str(uint16_t oc_i) {
		string pa = "";
		if (oc_pa(oc_i)) { pa = ", " + oc_sa_str(oc_i); } 
		return (pa);
	}
	
	// convert oc to pb string if pop
	string oc_pb_str(uint16_t oc_i) {
		string pb = "";
		if (oc_pb(oc_i)) { pb = ", " + oc_sb_str(oc_i); } 
		return (pb);
	}
	
	// test tokens for pop opcode
	bool tokens_are_pop(const vector<string>& token_i) {
		int32_t tokens = token_i.size();  // get token count
		if (!tokens) { return (false); }
		else {
			bool pop_f = true;
			for (int32_t i = 0; i < tokens; i++) { if (!str_is_pop(token_i[i])) { pop_f = false; } }
			return (pop_f);
		}
	}
	
	// test tokens for cls opcode
	bool tokens_are_cls(const vector<string>& token_i) {
		int32_t tokens = token_i.size();  // get token count
		if (!tokens) { return (false); }
		else {
			bool cls_f = true;
			for (int32_t i = 0; i < tokens; i++) { if (!str_is_cls(token_i[i])) { cls_f = false; } }
			return (cls_f);
		}
	}
	
	// string to tokens
	vector<string> str_to_tokens(const string& str_i) {
		stringstream str_ss(str_i);
		vector<string> vtoken;
		string token;
		while (str_ss >> token) { vtoken.push_back(token); }
		return (vtoken);
	}

	// tokens to string
	string tokens_to_str(const vector<string>& vtoken_i) {
		string str_buf;
		for (uint32_t i = 0; i < vtoken_i.size(); i++) { 
			str_buf += vtoken_i[i] + " ";
		}
		return (str_buf);
	}

	
	// encode opcode and immediate values given tokenized HAL string vector and current address
	bool hal_to_oc(vector<string> token_i, uint32_t addr_i, vector<uint8_t>& oc_o, int32_t& lits_o, string& error_msg_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		oc_o.clear();  // clear opcode
		lits_o = 0;  // clear lits
	
		// kill beginning of line # if present
		if (token_i.size() && str_is_int(token_i[0], "AUTO")) {
			vector<string>::iterator it = token_i.begin();
			token_i.erase(it);
		}
	
		// get token count
		int32_t tokens = token_i.size();
		
		if (tokens) {  // have tokens
			///////////////
			// nop & hlt //
			///////////////
			if ((tokens == 1) && (token_i[0] == "nop")) {
				// op byte
				oc_o.push_back(OP_NOP);  
			}
			else if ((tokens == 1) && (token_i[0] == "hlt")) {
				// op byte
				oc_o.push_back(OP_HLT);  
			}
			/////////////
			// one-hot //
			/////////////
			// pop
			else if (tokens_are_pop(token_i)) {
				// op byte
				oc_o.push_back(OP_POP);
				// im byte
				uint8_t oh_int = 0;
				for (int32_t i = 0; i < tokens; i++) { oh_int |= oh_to_int(token_i[i]); };
				oc_o.push_back(oh_int);
			}
			// cls
			else if (tokens_are_cls(token_i)) {
				// op byte
				oc_o.push_back(OP_CLS);
				// im byte
				uint8_t oh_int = 0;
				for (int32_t i = 0; i < tokens; i++) { oh_int |= oh_to_int(token_i[i]); };
				oc_o.push_back(oh_int);
			}
			///////////////
			// branching //
			///////////////
			// sa := pc := sb
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "pc") && (token_i[3] == ":=") && str_is_sab(token_i[4])) {
				oc_o.push_back(OP_GSB);  // op byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));  // sab byte
			}
			// sa := pc += sb
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "pc") && (token_i[3] == "+=") && str_is_sab(token_i[4])) {
				oc_o.push_back(OP_JSB);  // op byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));  // sab byte
			}
			// sa := pc := addr; optional Pb at end
			else if (((tokens == 5) || (tokens == 6)) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "pc") && (token_i[3] == ":=") && str_is_int(token_i[4], "AUTO")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[4], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				// op byte
				int32_t bytes = 1;  // default
				if (im8_f) { oc_o.push_back(OP_JSB_8); bytes = 1; }
				else if (im16_f) { oc_o.push_back(OP_JSB_16); bytes = 2; }
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 5) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }
				else if ((tokens == 6) && str_is_pop(token_i[5])) { oc_o.push_back(sab_to_int(token_i[0], token_i[5])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[5]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := pc := addr; optional Pb at end"; }
			}
			// ( sa %1 0 ) ? pc += sb; %1: ==, !=, <, !<
			else if ((tokens == 9) && (token_i[0] == "(") && str_is_sab(token_i[1]) && (token_i[3] == "0") && (token_i[4] == ")") && (token_i[5] == "?") && (token_i[6] == "pc") && (token_i[7] == "+=") && str_is_sab(token_i[8])) {
				// op byte
				if (token_i[2] == "==") { oc_o.push_back(OP_JMP_Z); }
				else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_NZ); }
				else if (token_i[2] == "<") { oc_o.push_back(OP_JMP_LZ); }
				else if (token_i[2] == "!<") { oc_o.push_back(OP_JMP_NLZ); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[1], token_i[8]));
				// errors
				if (error_f) { error_msg_o += ", pattern: ( sa %1 0 ) ? pc += sb; %1: ==, !=, <, !<"; }
			}
			// pc %1 sb; %1: +=, :=, :i=; optional pa at end
			else if (((tokens == 3) || (tokens == 4)) && (token_i[0] == "pc") && str_is_sab(token_i[2])) {
				// op byte
				if (token_i[1] == "+=") { oc_o.push_back(OP_JMP); }
				else if (token_i[1] == ":=") { oc_o.push_back(OP_GTO); }
				else if (token_i[1] == ":i=") { oc_o.push_back(OP_IRT); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				// sab byte
				if (tokens == 3) { oc_o.push_back(sab_to_int("s0", token_i[2])); }
				else if ((tokens == 4) && str_is_pop(token_i[3])) { oc_o.push_back(sab_to_int(token_i[3], token_i[2])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[3]; }
				// errors
				if (error_f) { error_msg_o += ", pattern: pc %1 sb; %1: +=, :=, :i=; optional pa at end"; }
			}
			// pc := addr
			else if ((tokens == 3) && (token_i[0] == "pc") && (token_i[1] == ":=") && str_is_int(token_i[2], "AUTO")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[2], "AUTO");
				int32_t im = addr - addr_i;
				im -= 2;  // im8 takes 2 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 3 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				if (!im16_f) { im--; }  // im24 takes 4 bytes total
				bool im24_f = (im >= -(1<<23)) && (im < (1<<23));
				// op byte
				int32_t bytes = 1;  // default
				if (im8_f) { oc_o.push_back(OP_JMP_8); bytes = 1; }
				else if (im16_f) { oc_o.push_back(OP_JMP_16); bytes = 2; }
				else if (im24_f) { oc_o.push_back(OP_JMP_24); bytes = 3; }
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: pc := addr"; }
			}
			// ( sa %1 odd ) ? pc := addr; %1: ==, !=; optional Pb at end
			else if (((tokens == 9) || (tokens == 10)) && (token_i[0] == "(") && str_is_sab(token_i[1]) && (token_i[3] == "odd") && (token_i[4] == ")") && (token_i[5] == "?") && (token_i[6] == "pc") && (token_i[7] == ":=") && str_is_int(token_i[8], "AUTO")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[8], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				// op byte
				int32_t bytes = 1;  // default
				if (im8_f) { 
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_8O); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_8NO); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_16O); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_16NO); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 9) { oc_o.push_back(sab_to_int(token_i[1], "s0")); }
				else if ((tokens == 10) && str_is_pop(token_i[9])) { oc_o.push_back(sab_to_int(token_i[1], token_i[9])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[9]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: ( sa %1 odd ) ? pc := addr; %1: ==, !=; optional Pb at end"; }
			}
			// ( sa %1 0 ) ? pc := addr; %1: ==, !=, <, !<; optional Pb at end
			else if (((tokens == 9) || (tokens == 10)) && (token_i[0] == "(") && str_is_sab(token_i[1]) && (token_i[3] == "0") && (token_i[4] == ")") && (token_i[5] == "?") && (token_i[6] == "pc") && (token_i[7] == ":=") && str_is_int(token_i[8], "AUTO")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[8], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				// op byte
				int32_t bytes = 1;  // default
				if (im8_f) { 
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_8Z); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_8NZ); }
					else if (token_i[2] == "<") { oc_o.push_back(OP_JMP_8LZ); }
					else if (token_i[2] == "!<") { oc_o.push_back(OP_JMP_8NLZ); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_16Z); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_16NZ); }
					else if (token_i[2] == "<") { oc_o.push_back(OP_JMP_16LZ); }
					else if (token_i[2] == "!<") { oc_o.push_back(OP_JMP_16NLZ); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 9) { oc_o.push_back(sab_to_int(token_i[1], "s0")); }
				else if ((tokens == 10) && str_is_pop(token_i[9])) { oc_o.push_back(sab_to_int(token_i[1], token_i[9])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[9]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: ( sa %1 0 ) ? pc := addr; %1: ==, !=, <, !<; optional Pb at end"; }
			}
			// ( sa %1 sb ) ? pc := addr; %1: ==, !=, <s, !<s, <u, !<u
			else if ((tokens == 9) && (token_i[0] == "(") && str_is_sab(token_i[1]) && str_is_sab(token_i[3]) && (token_i[4] == ")") && (token_i[5] == "?")  && (token_i[6] == "pc") && (token_i[7] == ":=") && str_is_int(token_i[8], "AUTO")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[8], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				// op byte
				int32_t bytes = 1;  // default
				if (im8_f) { 
					bytes = 1;
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_8E); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_8NE); }
					else if (token_i[2] == "<s") { oc_o.push_back(OP_JMP_8LS); }
					else if (token_i[2] == "!<s") { oc_o.push_back(OP_JMP_8NLS); }
					else if (token_i[2] == "<u") { oc_o.push_back(OP_JMP_8LU); }
					else if (token_i[2] == "!<u") { oc_o.push_back(OP_JMP_8NLU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[2] == "==") { oc_o.push_back(OP_JMP_16E); }
					else if (token_i[2] == "!=") { oc_o.push_back(OP_JMP_16NE); }
					else if (token_i[2] == "<s") { oc_o.push_back(OP_JMP_16LS); }
					else if (token_i[2] == "!<s") { oc_o.push_back(OP_JMP_16NLS); }
					else if (token_i[2] == "<u") { oc_o.push_back(OP_JMP_16LU); }
					else if (token_i[2] == "!<u") { oc_o.push_back(OP_JMP_16NLU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[1], token_i[3]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: ( sa %1 sb ) ? pc := addr; %1: ==, !=, <s, !<s, <u, !<u"; }
			}
			/////////////////////////////////////
			// logical, arithmetic, shift, etc //
			/////////////////////////////////////
			// sa %1 sb; %1: logical, arithmetic, shift
			else if ((tokens == 3) && str_is_sab(token_i[0]) && str_is_sab(token_i[2])) {
				// op byte
				if (token_i[1] == ":=") { oc_o.push_back(OP_CPY); }
				else if (token_i[1] == "&=") { oc_o.push_back(OP_AND); }
				else if (token_i[1] == "|=") { oc_o.push_back(OP_ORR); }
				else if (token_i[1] == "^=") { oc_o.push_back(OP_XOR); }
				else if (token_i[1] == "+=") { oc_o.push_back(OP_ADD_S); }  // default is signed!
				else if (token_i[1] == "+u=") { oc_o.push_back(OP_ADD_U); }
				else if (token_i[1] == "+us=") { oc_o.push_back(OP_ADD_US); }
				else if (token_i[1] == "+su=") { oc_o.push_back(OP_ADD_SU); }
				else if (token_i[1] == "+s=") { oc_o.push_back(OP_ADD_S); }
				else if (token_i[1] == "+xu=") { oc_o.push_back(OP_ADD_XU); }
				else if (token_i[1] == "+xus=") { oc_o.push_back(OP_ADD_XUS); }
				else if (token_i[1] == "+xsu=") { oc_o.push_back(OP_ADD_XSU); }
				else if (token_i[1] == "+xs=") { oc_o.push_back(OP_ADD_XS); }
				else if (token_i[1] == "-=") { oc_o.push_back(OP_SUB_S); }  // default is signed!
				else if (token_i[1] == "-u=") { oc_o.push_back(OP_SUB_U); }
				else if (token_i[1] == "-us=") { oc_o.push_back(OP_SUB_US); }
				else if (token_i[1] == "-su=") { oc_o.push_back(OP_SUB_SU); }
				else if (token_i[1] == "-s=") { oc_o.push_back(OP_SUB_S); }
				else if (token_i[1] == "-xu=") { oc_o.push_back(OP_SUB_XU); }
				else if (token_i[1] == "-xus=") { oc_o.push_back(OP_SUB_XUS); }
				else if (token_i[1] == "-xsu=") { oc_o.push_back(OP_SUB_XSU); }
				else if (token_i[1] == "-xs=") { oc_o.push_back(OP_SUB_XS); }
				else if (token_i[1] == "*=") { oc_o.push_back(OP_MUL_S); }  // default is signed!
				else if (token_i[1] == "*u=") { oc_o.push_back(OP_MUL_U); }
				else if (token_i[1] == "*us=") { oc_o.push_back(OP_MUL_US); }
				else if (token_i[1] == "*su=") { oc_o.push_back(OP_MUL_SU); }
				else if (token_i[1] == "*s=") { oc_o.push_back(OP_MUL_S); }
				else if (token_i[1] == "*xu=") { oc_o.push_back(OP_MUL_XU); }
				else if (token_i[1] == "*xus=") { oc_o.push_back(OP_MUL_XUS); }
				else if (token_i[1] == "*xsu=") { oc_o.push_back(OP_MUL_XSU); }
				else if (token_i[1] == "*xs=") { oc_o.push_back(OP_MUL_XS); }
				else if (token_i[1] == "<<r=") { oc_o.push_back(OP_ROL); }
				else if (token_i[1] == "<<=") { oc_o.push_back(OP_SHL_U); }
				else if (token_i[1] == "<<<=") { oc_o.push_back(OP_SHL_S); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[2]));  
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 sb; %1: logical, arithmetic, shift"; }
			}
			// sa := %1 sb; %1: ^, ~
			else if ((tokens == 4) && str_is_sab(token_i[0]) && (token_i[1] == ":=")  && !str_is_int(token_i[2], "AUTO") && str_is_sab(token_i[3])) {
				// op byte
				if (token_i[2] == "^") { oc_o.push_back(OP_BRX); }
				else if (token_i[2] == "~") { oc_o.push_back(OP_NOT); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[3]));  
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := %1 sb; %1: ^, ~"; }
			}
			// sa := %1 ( sb ); %1: nsb, sgn, swp, flp, lzc, lim, sat 
			else if ((tokens == 6) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[3] == "(") && str_is_sab(token_i[4]) && (token_i[5] == ")")) {
				// op byte
				if (token_i[2] == "nsb") { oc_o.push_back(OP_NSB); }
				else if (token_i[2] == "sgn") { oc_o.push_back(OP_SGN); }
				else if (token_i[2] == "swp") { oc_o.push_back(OP_SWP); }
				else if (token_i[2] == "flp") { oc_o.push_back(OP_FLP); }
				else if (token_i[2] == "lzc") { oc_o.push_back(OP_LZC); }
				else if (token_i[2] == "lim") { oc_o.push_back(OP_LIM); }
				else if (token_i[2] == "sat") { oc_o.push_back(OP_SAT); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[2]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := %1 ( sb );  %1: nsb, sgn, swp, flp, lzc, lim, sat"; }
			}
			// sa := 1 << sb
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "1")  && (token_i[3] == "<<") && str_is_sab(token_i[4])) {
				// op byte
				oc_o.push_back(OP_POW);  
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));
			}
			// sa := sb %1 sa; %1: subtract (reverse subtract)
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && str_is_sab(token_i[2]) && str_is_sab(token_i[4])) {
				// op byte
				if (token_i[3] == "-") { oc_o.push_back(OP_SBR_S); }  // default is signed! 
				else if (token_i[3] == "-u") { oc_o.push_back(OP_SBR_U); } 
				else if (token_i[3] == "-us") { oc_o.push_back(OP_SBR_US); } 
				else if (token_i[3] == "-su") { oc_o.push_back(OP_SBR_SU); } 
				else if (token_i[3] == "-s") { oc_o.push_back(OP_SBR_S); } 
				else if (token_i[3] == "-xu") { oc_o.push_back(OP_SBR_XU); } 
				else if (token_i[3] == "-xus") { oc_o.push_back(OP_SBR_XUS); } 
				else if (token_i[3] == "-xsu") { oc_o.push_back(OP_SBR_XSU); } 
				else if (token_i[3] == "-xs") { oc_o.push_back(OP_SBR_XS); } 
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[3]; }
				// sab byte
				if (str_is_pop(token_i[4])) { oc_o.push_back(sab_to_int(token_i[4], token_i[2])); }
				else { oc_o.push_back(sab_to_int(token_i[0], token_i[2])); }
				// errors
				if (token_i[0][1] != token_i[4][1]) { error_f = true; error_msg_o += "Stacks do not match: " + token_i[0] + ", " + token_i[4]; }
				if (error_f) { error_msg_o += ", pattern: sa := sb %1 sa; %1: subtract"; }
			}
			////////////
			// memory //
			////////////
			// sa %1 mem [ sb ]; %1: :=, :hs=, :hu=, :bs=, :bu=
			else if ((tokens == 6) && str_is_sab(token_i[0]) && (token_i[2] == "mem") && (token_i[3] == "[") && str_is_sab(token_i[4]) && (token_i[5] == "]") ) {
				// op byte
				if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_R); }  
				else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_RHS); }
				else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_RHU); }
				else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_RBS); }
				else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_RBU); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));  
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 mem [ sb ]; %1: :=, :hs=, :hu=, :bs=, :bu="; }
			}
			// sa %1 mem [ sb im ]; %1: :=, :hs=, :hu=, :bs=, :bu=
			else if ((tokens == 7) && str_is_sab(token_i[0]) && (token_i[2] == "mem") && (token_i[3] == "[") && str_is_sab(token_i[4]) && str_is_int(token_i[5], "AUTO") && (token_i[6] == "]")) {
				// im calcs
				int32_t im = str_to_int(token_i[5], "AUTO");
				bool im8u_f = (im >= 0) && (im < (1<<8));
				bool im16u_f = (im >= 0) && (im < (1<<16));
				int32_t bytes = 1;
				// op byte
				if (im8u_f) { 
					bytes = 1;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_8R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_8RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_8RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_8RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_8RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else if (im16u_f) { 
					bytes = 2;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_16R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_16RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_16RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_16RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_16RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[5]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 mem [ sb + im ]; %1: :=, :hs=, :hu=, :bs=, :bu="; }
			}
			// sa %1 mem [ addr ]; %1: :=, :hs=, :hu=, :bs=, :bu=; optional Pb at end
			else if (((tokens == 6) || (tokens == 7)) && str_is_sab(token_i[0]) && (token_i[2] == "mem") && (token_i[3] == "[") && str_is_int(token_i[4], "AUTO") && (token_i[5] == "]")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[4], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) { 
					bytes = 1;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_I8R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_I8RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_I8RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_I8RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_I8RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_I16R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_I16RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_I16RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_I16RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_I16RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 6) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 7) && str_is_pop(token_i[6])) { oc_o.push_back(sab_to_int(token_i[0], token_i[6])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[6]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 mem [ addr ]; %1: :=, :hs=, :hu=, :bs=, :bu=; optional Pb at end"; }
			}
			// sa %1 mem [ addr offs ]; %1: :=, :hs=, :hu=, :bs=, :bu=; optional Pb at end
			else if (((tokens == 7) || (tokens == 8)) && str_is_sab(token_i[0]) && (token_i[2] == "mem") && (token_i[3] == "[") && str_is_int(token_i[4], "AUTO") && str_is_int(token_i[5], "AUTO") && (token_i[6] == "]")) {
				// im calcs
				uint32_t addr = str_to_int(token_i[4], "AUTO");
				int32_t offs = str_to_int(token_i[5], "AUTO");
				int32_t im = addr - addr_i;
				im += offs;  // offset
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) { 
					bytes = 1;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_I8R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_I8RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_I8RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_I8RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_I8RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[1] == ":=") { oc_o.push_back(OP_MEM_I16R); } 
					else if (token_i[1] == ":hs=") { oc_o.push_back(OP_MEM_I16RHS); }
					else if (token_i[1] == ":hu=") { oc_o.push_back(OP_MEM_I16RHU); }
					else if (token_i[1] == ":bs=") { oc_o.push_back(OP_MEM_I16RBS); }
					else if (token_i[1] == ":bu=") { oc_o.push_back(OP_MEM_I16RBU); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 7) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 8) && str_is_pop(token_i[7])) { oc_o.push_back(sab_to_int(token_i[0], token_i[7])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[7]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 mem [ addr offs ]; %1: :=, :hs=, :hu=, :bs=, :bu=; optional Pb at end"; }
			}
			// mem [ sb ] %1 sa; %1: :=, :h=, :b=
			else if ((tokens == 6) && (token_i[0] == "mem") && (token_i[1] == "[") && str_is_sab(token_i[2]) && (token_i[3] == "]") && str_is_sab(token_i[5])) {
				// op byte
				if (token_i[4] == ":=") { oc_o.push_back(OP_MEM_W); }
				else if (token_i[4] == ":h=") { oc_o.push_back(OP_MEM_WH); }
				else if (token_i[4] == ":b=") { oc_o.push_back(OP_MEM_WB); }
				else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[4]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[5], token_i[2]));
				// errors
				if (error_f) { error_msg_o += ", pattern: mem [ sb ] %1 sa; %1: :=, :h=, :b="; }
			}
			// mem [ sb im ] %1 sa; %1: :=, :h=, :b=
			else if ((tokens == 7) && (token_i[0] == "mem") && (token_i[1] == "[") && str_is_sab(token_i[2]) && str_is_int(token_i[3], "AUTO") && (token_i[4] == "]") && str_is_sab(token_i[6])) {
				// im calcs
				int32_t im = str_to_int(token_i[3], "AUTO");
				bool im8u_f = (im >= 0) && (im < (1<<8));
				bool im16u_f = (im >= 0) && (im < (1<<16));
				int32_t bytes = 1;
				// op byte
				if (im8u_f) { 
					bytes = 1;
					if (token_i[5] == ":=") { oc_o.push_back(OP_MEM_8W); }  // op byte
					else if (token_i[5] == ":h=") { oc_o.push_back(OP_MEM_8WH); }
					else if (token_i[5] == ":b=") { oc_o.push_back(OP_MEM_8WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[5]; }
				}
				else if (im16u_f) { 
					bytes = 2;
					if (token_i[5] == ":=") { oc_o.push_back(OP_MEM_16W); }  // op byte
					else if (token_i[5] == ":h=") { oc_o.push_back(OP_MEM_16WH); }
					else if (token_i[5] == ":b=") { oc_o.push_back(OP_MEM_16WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[5]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[3]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[6], token_i[2]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: mem [ sb im ] %1 sa; %1: :=, :l=, :b="; }
			}
			// mem [ addr ] %1 sa; %1: :=, :h=, :b=; optional Pb at end
			else if (((tokens == 6) || (tokens == 7)) && (token_i[0] == "mem") && (token_i[1] == "[") && str_is_int(token_i[2], "AUTO") && (token_i[3] == "]") && str_is_sab(token_i[5])) {
				// im calcs
				uint32_t addr = str_to_int(token_i[2], "AUTO");
				int32_t im = addr - addr_i;
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) { 
					bytes = 1;
					if (token_i[4] == ":=") { oc_o.push_back(OP_MEM_I8W); }  // op byte
					else if (token_i[4] == ":h=") { oc_o.push_back(OP_MEM_I8WH); }
					else if (token_i[4] == ":b=") { oc_o.push_back(OP_MEM_I8WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[4]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[4] == ":=") { oc_o.push_back(OP_MEM_I16W); }  // op byte
					else if (token_i[4] == ":h=") { oc_o.push_back(OP_MEM_I16WH); }
					else if (token_i[4] == ":b=") { oc_o.push_back(OP_MEM_I16WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[4]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 6) { oc_o.push_back(sab_to_int(token_i[5], "s0")); }  
				else if ((tokens == 7) && str_is_pop(token_i[6])) { oc_o.push_back(sab_to_int(token_i[5], token_i[6])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[6]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: mem [ addr ] %1 sa; %1: :=, :l=, :b=; optional Pb at end"; }
			}
			// mem [ addr offs ] %1 sa; %1: :=, :h=, :b=; optional Pb at end
			else if (((tokens == 7) || (tokens == 8)) && (token_i[0] == "mem") && (token_i[1] == "[") && str_is_int(token_i[2], "AUTO") && str_is_int(token_i[3], "AUTO") && (token_i[4] == "]") && str_is_sab(token_i[6])) {
				// im calcs
				uint32_t addr = str_to_int(token_i[2], "AUTO");
				int32_t offs = str_to_int(token_i[3], "AUTO");
				int32_t im = addr - addr_i;
				im += offs;  // offset
				im -= 3;  // im8 takes 3 bytes total
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				if (!im8_f) { im--; }  // im16 takes 4 bytes total
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) { 
					bytes = 1;
					if (token_i[5] == ":=") { oc_o.push_back(OP_MEM_I8W); }  // op byte
					else if (token_i[5] == ":h=") { oc_o.push_back(OP_MEM_I8WH); }
					else if (token_i[5] == ":b=") { oc_o.push_back(OP_MEM_I8WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[5]; }
				}
				else if (im16_f) { 
					bytes = 2;
					if (token_i[5] == ":=") { oc_o.push_back(OP_MEM_I16W); }  // op byte
					else if (token_i[5] == ":h=") { oc_o.push_back(OP_MEM_I16WH); }
					else if (token_i[5] == ":b=") { oc_o.push_back(OP_MEM_I16WB); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[5]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + int_to_str(im, "INT", ALU_W, true); }
				// sab byte
				if (tokens == 7) { oc_o.push_back(sab_to_int(token_i[6], "s0")); }  
				else if ((tokens == 8) && str_is_pop(token_i[7])) { oc_o.push_back(sab_to_int(token_i[6], token_i[7])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[7]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: mem [ addr offs ] %1 sa; %1: :=, :l=, :b=; optional Pb at end"; }
			}
			//////////////
			// register //
			//////////////
			// sa := reg [ sb ]
			else if ((tokens == 6) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "reg") && (token_i[3] == "[") && str_is_sab(token_i[4]) && (token_i[5] == "]") ) {
				// op byte
				oc_o.push_back(OP_REG_R);  
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));  
			}
			// sa := reg [ im ]; optional Pb at end
			else if (((tokens == 6) || (tokens == 7)) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "reg") && (token_i[3] == "[") && str_is_int(token_i[4], "AUTO") && (token_i[5] == "]") ) {
				// im calcs
				int32_t im = str_to_int(token_i[4], "AUTO");
				bool im8u_f = (im >= 0) && (im < (1<<8));
				// op byte
				oc_o.push_back(OP_REG_8R);
				// sab byte
				if (tokens == 6) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 7) && str_is_pop(token_i[6])) { oc_o.push_back(sab_to_int(token_i[0], token_i[6])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[6]; }
				// im byte
				oc_o.push_back(im);
				// errors
				if (!im8u_f) { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[4]; }
				if (error_f) { error_msg_o += ", pattern: sa := reg [ im ]; optional Pb at end"; }
			}
			// reg [ sb ] := sa
			else if ((tokens == 6) && (token_i[0] == "reg") && (token_i[1] == "[") && str_is_sab(token_i[2]) && (token_i[3] == "]") && (token_i[4] == ":=" ) && str_is_sab(token_i[5])) {
				// op byte
				oc_o.push_back(OP_REG_W); 
				// sab byte
				oc_o.push_back(sab_to_int(token_i[5], token_i[2]));  
			}
			// reg [ im ] := sa; optional Pb at end
			else if (((tokens == 6) || (tokens == 7)) && (token_i[0] == "reg") && (token_i[1] == "[") && str_is_int(token_i[2], "AUTO") && (token_i[3] == "]") && (token_i[4] == ":=") && str_is_sab(token_i[5])) {
				// im calcs
				int32_t im = str_to_int(token_i[2], "AUTO");
				bool im8u_f = (im >= 0) && (im < (1<<8));
				// op byte
				oc_o.push_back(OP_REG_8W); 
				// sab byte
				if (tokens == 6) { oc_o.push_back(sab_to_int(token_i[5], "s0")); }  
				else if ((tokens == 7) && str_is_pop(token_i[6])) { oc_o.push_back(sab_to_int(token_i[5], token_i[6])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[6]; }
				// im byte
				oc_o.push_back(im);
				// errors
				if (!im8u_f) { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[2]; }
				if (error_f) { error_msg_o += ", pattern: reg [ im ] := sa; optional Pb at end"; }
			}
			///////////////////
			// immediate lit //
			///////////////////
			// sa := im; optional Pb at end
			else if (((tokens == 3) || (tokens == 4)) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && str_is_int(token_i[2], "AUTO")) {
				// im calcs
				int64_t im = str_to_int(token_i[2], "AUTO");
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				bool im8u_f = (im >= 0) && (im < (1<<8));
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				bool im16u_f = (im >= 0) && (im < (1<<16));
				bool im32_f = (im >= -(int64_t(1)<<31)) && (im < (int64_t(1)<<32));
				int32_t bytes = 1;
				// op byte
				if (im8_f) { oc_o.push_back(OP_LIT_BS); bytes = 1; }  
				else if (im8u_f) { oc_o.push_back(OP_LIT_BU); bytes = 1; }  
				else if (im16_f) { oc_o.push_back(OP_LIT_HS); bytes = 2; }  
				else if (im16u_f) { oc_o.push_back(OP_LIT_HU); bytes = 2; }  
				else if (im32_f) { oc_o.push_back(OP_LIT); bytes = 4; }  
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[2]; }
				// sab byte
				if (tokens == 3) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 4) && str_is_pop(token_i[3])) { oc_o.push_back(sab_to_int(token_i[0], token_i[3])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[3]; }
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := im; optional Pb at end"; }
			}
			/////////////////////////////////////
			// immediate shift, arith, logical //
			/////////////////////////////////////
			// sa %1 im; %1: arithmetic, shift, logical (but not :=)
			else if ((tokens == 3) && str_is_sab(token_i[0]) && (token_i[1] != ":=" ) && str_is_int(token_i[2], "AUTO")) {
				// im calcs
				int32_t im = str_to_int(token_i[2], "AUTO");
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) {
					if (token_i[1] == "+=") { oc_o.push_back(OP_ADD_8S); }  // default is signed!
					else if (token_i[1] == "+u=") { oc_o.push_back(OP_ADD_8U); }
					else if (token_i[1] == "+us=") { oc_o.push_back(OP_ADD_8US); }
					else if (token_i[1] == "+su=") { oc_o.push_back(OP_ADD_8SU); }
					else if (token_i[1] == "+s=") { oc_o.push_back(OP_ADD_8S); }
					else if (token_i[1] == "-=") { oc_o.push_back(OP_SUB_8S); }  // default is signed!
					else if (token_i[1] == "-u=") { oc_o.push_back(OP_SUB_8U); }
					else if (token_i[1] == "-us=") { oc_o.push_back(OP_SUB_8US); }
					else if (token_i[1] == "-su=") { oc_o.push_back(OP_SUB_8SU); }
					else if (token_i[1] == "-s=") { oc_o.push_back(OP_SUB_8S); }
					else if (token_i[1] == "*=") { oc_o.push_back(OP_MUL_8S); }  // default is signed!
					else if (token_i[1] == "*u=") { oc_o.push_back(OP_MUL_8U); }
					else if (token_i[1] == "*us=") { oc_o.push_back(OP_MUL_8US); }
					else if (token_i[1] == "*su=") { oc_o.push_back(OP_MUL_8SU); }
					else if (token_i[1] == "*s=") { oc_o.push_back(OP_MUL_8S); }
					else if (token_i[1] == "<<=") { oc_o.push_back(OP_SHL_8U); }
					else if (token_i[1] == "<<<=") { oc_o.push_back(OP_SHL_8S); }
					else if (token_i[1] == "<<r=") { oc_o.push_back(OP_ROL_8); }
					else if (token_i[1] == "&=") { oc_o.push_back(OP_AND_8); }
					else if (token_i[1] == "|=") { oc_o.push_back(OP_ORR_8); }
					else if (token_i[1] == "^=") { oc_o.push_back(OP_XOR_8); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else if (im16_f) {
					bytes = 2;
					if (token_i[1] == "+=") { oc_o.push_back(OP_ADD_16S); }  // default is signed!
					else if (token_i[1] == "+u=") { oc_o.push_back(OP_ADD_16U); }
					else if (token_i[1] == "+us=") { oc_o.push_back(OP_ADD_16US); }
					else if (token_i[1] == "+su=") { oc_o.push_back(OP_ADD_16SU); }
					else if (token_i[1] == "+s=") { oc_o.push_back(OP_ADD_16S); }
					else if (token_i[1] == "-=") { oc_o.push_back(OP_SUB_16S); }  // default is signed!
					else if (token_i[1] == "-u=") { oc_o.push_back(OP_SUB_16U); }
					else if (token_i[1] == "-us=") { oc_o.push_back(OP_SUB_16US); }
					else if (token_i[1] == "-su=") { oc_o.push_back(OP_SUB_16SU); }
					else if (token_i[1] == "-s=") { oc_o.push_back(OP_SUB_16S); }
					else if (token_i[1] == "*=") { oc_o.push_back(OP_MUL_16S); }  // default is signed!
					else if (token_i[1] == "*u=") { oc_o.push_back(OP_MUL_16U); }
					else if (token_i[1] == "*us=") { oc_o.push_back(OP_MUL_16US); }
					else if (token_i[1] == "*su=") { oc_o.push_back(OP_MUL_16SU); }
					else if (token_i[1] == "*s=") { oc_o.push_back(OP_MUL_16S); }
					else if (token_i[1] == "&=") { oc_o.push_back(OP_AND_16); }
					else if (token_i[1] == "|=") { oc_o.push_back(OP_ORR_16); }
					else if (token_i[1] == "^=") { oc_o.push_back(OP_XOR_16); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[1]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[2]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[0]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa %1 im; %1: arithmetic, shift, logical (but not :=)"; }
			}
			// sa := sb %1 im; %1: arithmetic, shift, logical
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=" ) && str_is_sab(token_i[2]) && str_is_int(token_i[4], "AUTO")) {
				// im calcs
				int32_t im = str_to_int(token_i[4], "AUTO");
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) {
					if (token_i[3] == "+") { oc_o.push_back(OP_ADD_8S); }  // default is signed!
					else if (token_i[3] == "+u") { oc_o.push_back(OP_ADD_8U); } 
					else if (token_i[3] == "+us") { oc_o.push_back(OP_ADD_8US); } 
					else if (token_i[3] == "+su") { oc_o.push_back(OP_ADD_8SU); } 
					else if (token_i[3] == "+s") { oc_o.push_back(OP_ADD_8S); } 
					else if (token_i[3] == "-") { oc_o.push_back(OP_SUB_8S); }  // default is signed! 
					else if (token_i[3] == "-u") { oc_o.push_back(OP_SUB_8U); } 
					else if (token_i[3] == "-us") { oc_o.push_back(OP_SUB_8US); } 
					else if (token_i[3] == "-su") { oc_o.push_back(OP_SUB_8SU); } 
					else if (token_i[3] == "-s") { oc_o.push_back(OP_SUB_8S); } 
					else if (token_i[3] == "*") { oc_o.push_back(OP_MUL_8S); }  // default is signed!
					else if (token_i[3] == "*u") { oc_o.push_back(OP_MUL_8U); }
					else if (token_i[3] == "*us") { oc_o.push_back(OP_MUL_8US); }
					else if (token_i[3] == "*su") { oc_o.push_back(OP_MUL_8SU); }
					else if (token_i[3] == "*s") { oc_o.push_back(OP_MUL_8S); }
					else if (token_i[3] == "<<") { oc_o.push_back(OP_SHL_8U); }
					else if (token_i[3] == "<<<") { oc_o.push_back(OP_SHL_8S); }
					else if (token_i[3] == "<<r") { oc_o.push_back(OP_ROL_8); }
					else if (token_i[3] == "&") { oc_o.push_back(OP_AND_8); }
					else if (token_i[3] == "|") { oc_o.push_back(OP_ORR_8); }
					else if (token_i[3] == "^") { oc_o.push_back(OP_XOR_8); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[3]; }
				}
				else if (im16_f) {
					bytes = 2;
					if (token_i[3] == "+") { oc_o.push_back(OP_ADD_16S); }   // default is signed!
					else if (token_i[3] == "+u") { oc_o.push_back(OP_ADD_16U); } 
					else if (token_i[3] == "+us") { oc_o.push_back(OP_ADD_16US); } 
					else if (token_i[3] == "+su") { oc_o.push_back(OP_ADD_16SU); } 
					else if (token_i[3] == "+s") { oc_o.push_back(OP_ADD_16S); } 
					else if (token_i[3] == "-") { oc_o.push_back(OP_SUB_16S); }   // default is signed!
					else if (token_i[3] == "-u") { oc_o.push_back(OP_SUB_16U); } 
					else if (token_i[3] == "-us") { oc_o.push_back(OP_SUB_16US); } 
					else if (token_i[3] == "-su") { oc_o.push_back(OP_SUB_16SU); } 
					else if (token_i[3] == "-s") { oc_o.push_back(OP_SUB_16S); } 
					else if (token_i[3] == "*") { oc_o.push_back(OP_MUL_16S); }  // default is signed!
					else if (token_i[3] == "*u") { oc_o.push_back(OP_MUL_16U); }
					else if (token_i[3] == "*us") { oc_o.push_back(OP_MUL_16US); }
					else if (token_i[3] == "*su") { oc_o.push_back(OP_MUL_16SU); }
					else if (token_i[3] == "*s") { oc_o.push_back(OP_MUL_16S); }
					else if (token_i[3] == "&") { oc_o.push_back(OP_AND_16); }
					else if (token_i[3] == "|") { oc_o.push_back(OP_ORR_16); }
					else if (token_i[3] == "^") { oc_o.push_back(OP_XOR_16); }
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[3]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[4]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[2]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := sb %1 im; %1: arithmetic, shift, logical"; }
			}
			// sa := im %1 sb; %1: subtract (reverse subtract)
			else if ((tokens == 5) && str_is_sab(token_i[0]) && (token_i[1] == ":=" ) && str_is_int(token_i[2], "AUTO") && str_is_sab(token_i[4])) {
				// im calcs
				int32_t im = str_to_int(token_i[2], "AUTO");
				bool im8_f = (im >= -(1<<7)) && (im < (1<<7));
				bool im16_f = (im >= -(1<<15)) && (im < (1<<15));
				int32_t bytes = 1;
				// op byte
				if (im8_f) {
					if (token_i[3] == "-") { oc_o.push_back(OP_SBR_8S); }  // default is signed! 
					else if (token_i[3] == "-u") { oc_o.push_back(OP_SBR_8U); } 
					else if (token_i[3] == "-us") { oc_o.push_back(OP_SBR_8US); } 
					else if (token_i[3] == "-su") { oc_o.push_back(OP_SBR_8SU); } 
					else if (token_i[3] == "-s") { oc_o.push_back(OP_SBR_8S); } 
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[3]; }
				}
				else if (im16_f) {
					bytes = 2;
					if (token_i[3] == "-") { oc_o.push_back(OP_SBR_16S); }   // default is signed!
					else if (token_i[3] == "-u") { oc_o.push_back(OP_SBR_16U); } 
					else if (token_i[3] == "-us") { oc_o.push_back(OP_SBR_16US); } 
					else if (token_i[3] == "-su") { oc_o.push_back(OP_SBR_16SU); } 
					else if (token_i[3] == "-s") { oc_o.push_back(OP_SBR_16S); } 
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[3]; }
				}
				else { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[2]; }
				// sab byte
				oc_o.push_back(sab_to_int(token_i[0], token_i[4]));
				// im byte(s)
				for (int32_t b = 0; b < bytes; b++) { oc_o.push_back(im>>(b*8)); }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := im %1 sb; %1: subtract"; }
			}
			// sa := 1 << im; optional Pb at end
			else if (((tokens == 5) || (tokens == 6)) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "1")  && (token_i[3] == "<<") && str_is_int(token_i[4], "AUTO")) {
				// im calcs
				int32_t im = str_to_int(token_i[4], "AUTO");
				bool im8u_f = (im >= 0) && (im < (1<<8));
				// op byte
				oc_o.push_back(OP_POW_8);
				// sab byte
				if (tokens == 5) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 6) && str_is_pop(token_i[5])) { oc_o.push_back(sab_to_int(token_i[0], token_i[5])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[5]; }
				// im byte
				oc_o.push_back(im);
				// errors
				if (!im8u_f) { error_f = true; error_msg_o += "Immediate value out of range: " + token_i[4]; }
				if (error_f) { error_msg_o += ", pattern: sa := 1 << im; optional Pb at end"; }
			}
			// sa := pc; optional Pb at end
			else if (((tokens == 3) || (tokens == 4)) && str_is_sab(token_i[0]) && (token_i[1] == ":=") && (token_i[2] == "pc")) {
				// op byte
				oc_o.push_back(OP_PCR);
				// sab byte
				if (tokens == 3) { oc_o.push_back(sab_to_int(token_i[0], "s0")); }  
				else if ((tokens == 4) && str_is_pop(token_i[3])) { oc_o.push_back(sab_to_int(token_i[0], token_i[3])); }
				else { error_f = true; error_msg_o += "Expecting stack pop: " + token_i[3]; }
				// errors
				if (error_f) { error_msg_o += ", pattern: sa := pc; optional Pb at end"; }
			}
			//////////////////
			// reserved mem //
			//////////////////
			// mem [ #s ] %1 ( #0, #1, ... ); %1: :=, :h=, :b=
			else if ((tokens >= 8) && (token_i[0] == "mem") && (token_i[1] == "[") && str_is_int(token_i[2], "AUTO") && (token_i[3] == "]") && (token_i[5] == "(") && str_is_int(token_i[6], "AUTO") && (token_i[tokens-1] == ")")) {
				// im calcs
				lits_o = str_to_int(token_i[2], "AUTO");
				if (lits_o <= 0) { error_f = true; error_msg_o += "Index must be positive: " + token_i[2]; }
				else {
					int32_t bytes = 4;
					if (token_i[4] == ":=") { bytes = 4; }
					else if (token_i[4] == ":h=") { bytes = 2; }
					else if (token_i[4] == ":b=") { bytes = 1; } 
					else { error_f = true; error_msg_o += "Unrecognized symbol: " + token_i[4]; }
					// loop thru data tokens
					int32_t im = 0;
					for (int32_t lit = 0; lit < lits_o; lit++) { 
						if (lit+6 < tokens-1) { im = str_to_int(token_i[lit+6], "AUTO"); }
						for (int32_t b = 0; b < bytes; b++) { 
							oc_o.push_back(im>>(b*8));
						}
					};
				}
				// errors
				if (error_f) { error_msg_o += ", pattern: mem [ #s ] %1 ( #0, #1, ... ); %1: :=, :h=, :b="; }
			}
			/////////////
			// default //
			/////////////
			// finally, no pattern match is a parse error
			else { error_f = true; error_msg_o += "No pattern match"; }
	
		}
	
		if (error_f) { error_msg_o = "Error Parse : " + error_msg_o; }
		return (error_f);
	}


	// Split HAL line into code and eolc, handle line state i/o
	bool hal_split_line(const string& hal_i, string& code_o, string& eolc_o, bool& blkc_f, string& error_msg_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		bool eolc_f = false;  // EOL comment flag
		code_o.clear();  // clear outputs
		eolc_o.clear();
		
		// buffer code and eol comments separately
		for (uint32_t i = 0; i < hal_i.length(); i++) {  // loop thru non-blank line
			// processing code
			if (!eolc_f && !blkc_f) {
				if ((i > 0) && (hal_i[i-1] == '/') && (hal_i[i] == '/')) {  // start of EOL comment
					code_o.pop_back();  // kill previous downstream char
					eolc_f = true;  // change state
				}
				else if ((i > 0) && (hal_i[i-1] == '/') && (hal_i[i] == '*')) {  // start of block comment
					code_o.pop_back();  // kill previous downstream char
					blkc_f = true;  // change state
				}
				else if ((i > 0) && (hal_i[i-1] == '*') && (hal_i[i] == '/')) {  // end of block comment is an error
					error_f = true; 
					error_msg_o += "Unexpected end of block comment"; 
				}
				else {   // else buffer code
					code_o += hal_i[i]; 
				}
			}
			// processing EOL comment
			else if (eolc_f && !blkc_f) {  
				eolc_o += hal_i[i];  // buffer eol comment
			}
			// processing block comment
			else if (!eolc_f && blkc_f) {  
				if ((i > 0) && (hal_i[i-1] == '*') && (hal_i[i] == '/')) {  // end of block comment
					blkc_f = false;  // change state
				}
			}
		} 
		
		// trim whitespace off ends
		code_o = str_trim(code_o);
		eolc_o = str_trim(eolc_o);
		
		return (error_f);
	}



	// HAL pre-processing: 
	// pull in all HAL files
	// split code and eolc, store in vsrc container
	// implement `include directives
	bool hal_pre_0(vector<src_t>& vsrc_o, string& error_msg_o, uint32_t& idx_o, uint32_t& files_o, uint32_t& dupes_o, const string& file_i) {

		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		bool done_f = false;  // clear done flag
		ifstream rd_file;  // input file stream
		string rd_buf;  // line buffer
		files_o = 0;  // init file count
		dupes_o = 0;  // init dup file count
		idx_o = 0;  // init idx

		// state
		struct state_t {
	   		string file = "";
			uint32_t line = 0;
			fstream::pos_type pos = 0;
			bool blkc_f = false;
		};
		
		// state holders
		unordered_set<string> set;  // file name store
		vector<state_t> vstate;  // state store
		state_t empty_state;  // empty state to push
	
		// prime things
		vstate.push_back( empty_state );
		vstate.back().file = file_i;
		set.insert(file_i);
		rd_file.open(vstate.back().file.c_str(), ios::in | ios::binary);  // open file stream
		if (!rd_file.is_open()) { error_f = true; error_msg_o += "Unable to open base file: " + vstate.back().file; }
		else { files_o++; }  // update file count

		while (!error_f && !done_f) {  // do until error or done with all files
		
			while (!error_f && getline(rd_file, rd_buf)) {  // do until error or end of input file is reached
			
				// declare stuff
				src_t empty_src;  // empty source
				string code_buf;
				
				// new line
				idx_o++;
				vstate.back().line++;
				vsrc_o.push_back( empty_src );  // new src line
				vsrc_o.back().file = vstate.back().file;
				vsrc_o.back().line = vstate.back().line;

				// split line
				error_f = hal_split_line(rd_buf, vsrc_o.back().code, vsrc_o.back().eolc, vstate.back().blkc_f, error_msg_o);

				// insert space before ` and before and after "
				for (uint32_t i = 0; i < vsrc_o.back().code.length(); i++) {
					if (vsrc_o.back().code[i] == '`') { code_buf += " `"; }
					else if (vsrc_o.back().code[i] == '\"') { code_buf += " \" "; }
					else { code_buf += vsrc_o.back().code[i]; }
				}
				
				// tokenize
				vector<string> vtoken = str_to_tokens(code_buf); 

				// change state
				if (!error_f && vtoken.size() && (str_to_lower(vtoken[0]) == "`include")) {  // `include found at sol
					if ((vtoken.size() == 4) && (vtoken[1] == "\"") && (vtoken[3] == "\"")) {  // properly formed
						// check for duplicate global file names
						if (!set.count(vtoken[2])) { set.insert(vtoken[2]); files_o++; }  // add file name
						else { dupes_o++; }  // else a dupe
						// check for duplicate state file names
						bool dupe_f = false;
						for (uint32_t s = 0; !error_f && (s < vstate.size()); s++) {  
							if (vstate[s].file == vtoken[2]) { dupe_f = true; }
						}
						if (!dupe_f) {  // not a state dupe
							vstate.back().pos = rd_file.tellg();  // save current position
							vstate.push_back( empty_state );  // new state
							vstate.back().file = vtoken[2];  // set new file name
							rd_file.close();  // close file stream
							rd_file.open(vstate.back().file.c_str(), ios::in | ios::binary);  // open new file stream
							if (!rd_file.is_open()) { error_f = true; error_msg_o += "Unable to open file: " + vstate.back().file; } 
						}
						else { error_f = true; error_msg_o += "Recursive `include file: " + vtoken[2]; }
					}
					else { error_f = true; error_msg_o += "Bad `include syntax"; }  // improperly formed
				}
	
			}  // end of input file parsing			
			
			if (rd_file.is_open()) { rd_file.close(); }  // close file if open
			
			// change state
			if (!error_f && !vstate.empty()) { vstate.pop_back(); } // pop previous state

			if (!error_f && !vstate.empty()) {
				rd_file.open(vstate.back().file.c_str(), ios::in | ios::binary);  // reopen previous file
				rd_file.seekg(vstate.back().pos);  // reposition
			}
			else { done_f = true; }

		}
		
		if (error_f) { error_msg_o = "\nError PRE_0 : " + error_msg_o;  }
		return (error_f);
	}


	// HAL pre-processing:
	// kill `include lines
	// replace double quoted string with individual single quoted ASCII chars
	bool hal_pre_1(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	
		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  

			// declare & init stuff	
			string code_rd_buf = vsrc_io[idx_o].code;
			string code_wr_buf;  // blank

			// tokenize
			vector<string> vtoken = str_to_tokens(code_rd_buf); 

			// check for `include at sol
			if (vtoken.size() && (str_to_lower(vtoken[0]) == "`include")) { code_rd_buf.clear(); }

			// string processing - replace string with chars
			uint32_t i = 0;
			bool str_f = false;  // flag
			while (!error_f && (i < code_rd_buf.length())) {  // loop thru non-blank line

				if (code_rd_buf[i] == '\"') {  // double quote (consume)
					str_f = !str_f;  // change state
					i++;  // next ch
				}
				else if (str_f) {  // processing string
					if (i < code_rd_buf.length()) {
						code_wr_buf += " \'";  // space & single open quote
						code_wr_buf += code_rd_buf[i];
						if (code_rd_buf[i] == '\\') {  // backslash (escape ch)
							i++;  // next ch
							if (i < code_rd_buf.length()) {
								code_wr_buf += code_rd_buf[i];
							}
							else { 
								error_f = true; 
								error_msg_o += "String error";
							}
						}
						code_wr_buf += "\' ";  // single close quote & space
						i++;  // next ch
					}
					else { 
						error_f = true; 
						error_msg_o += "String error";
					}
				}
				else {  // not processing string
					code_wr_buf += code_rd_buf[i];  // just copy
					i++;  // next ch
				}
					
			}  // end of string processing

			if (str_f) { 
				error_f = true; 
				error_msg_o += "Missing string close char";
			}
	
			// copy write buffer to output
			vsrc_io[idx_o].code = code_wr_buf;
	
		}  // end of input parsing
		
		if (error_f) { error_msg_o = "\nError PRE_1 : " + error_msg_o; }
		return (error_f);
	}



	// HAL pre-processing: 
	// replace ASCII with value
	bool hal_pre_2(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	
		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// declare & init stuff	
			string code_rd_buf = vsrc_io[idx_o].code;
			string code_wr_buf;  // blank

			// ASCII processing - replace single quote bracketed ASCII chars (up to 4 chars contiguous) with actual value
			uint32_t i = 0;
			bool ascii_f = false;  // flag
			uint32_t ascii_sum = 0;
			while (!error_f && (i < code_rd_buf.length())) {  // loop thru non-blank line

				if (code_rd_buf[i] == '\'') {  // single quote (consume)
					ascii_f = !ascii_f;  // change state
					i++;  // next ch
					if (!ascii_f) {  // ASCII end
						code_wr_buf += int_to_str(ascii_sum, "HEX", ALU_W, true);  // update with sum value
						ascii_sum = 0;  // reset sum
					}
				}
				else if (ascii_f) {  // processing ASCII
					ascii_sum *= 256;  // shift
					if (i < code_rd_buf.length()) {
						if (code_rd_buf[i] == '\\') {  // backslash (escape ch)
							i++;  // next ch
							if (i < code_rd_buf.length()) {
								if (code_rd_buf[i] == 'b') { ascii_sum += '\b'; }  // backspace
								else if (code_rd_buf[i] == 'f') { ascii_sum += '\f'; }  // formfeed
								else if (code_rd_buf[i] == 'n') { ascii_sum += '\n'; }  // newline
								else if (code_rd_buf[i] == 'r') { ascii_sum += '\r'; }  // return
								else if (code_rd_buf[i] == 't') { ascii_sum += '\t'; }  // tab
								else if (code_rd_buf[i] == '\"') { ascii_sum += '\"'; }  // double quote
								else if (code_rd_buf[i] == '\'') { ascii_sum += '\''; }  // single quote
								else if (code_rd_buf[i] == '\\') { ascii_sum += '\\'; }  // backslash
								else { 
									error_f = true; 
									error_msg_o += "Unsupported ASCII escape char: " + code_rd_buf[i];
								}
								i++;  // next ch
							}
							else { 
								error_f = true; 
								error_msg_o += "ASCII error";
							}
						}
						else {  // normal ASCII
							ascii_sum += code_rd_buf[i];  // sum
							i++;  // next ch
						}
					}
					else { 
						error_f = true; 
						error_msg_o += "ASCII error";
					}
				}
				else {  // not processing ASCII
					code_wr_buf += code_rd_buf[i];  // just copy
					i++;  // next ch
				}
					
			}  // end of ASCII processing

			if (ascii_f) { 
				error_f = true; 
				error_msg_o += "Missing ASCII close char";
			}
	
			// copy write buffer to output
			vsrc_io[idx_o].code = code_wr_buf;
	
		}  // end of input parsing
		
		if (error_f) { error_msg_o = "\nError PRE_2 : " + error_msg_o; }
		return (error_f);
	}



	// HAL pre-processing: 
	// insert spaces around certain groupings
	bool hal_pre_3(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	
		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  

			// declare & init stuff	
			string code_rd_buf = vsrc_io[idx_o].code;

			// search for & insert space around HAL symbols, replace commas with spaces
			uint32_t i = 0;
			string code_wr_buf;  // blank
			while (!error_f && (i < code_rd_buf.length())) {  // loop thru non-blank line

				string sub_str;
				int32_t sub_len = 0;
				bool done_f = false;

				sub_len = 5;  // 5 chars sub_str
				sub_str = code_rd_buf.substr(i, sub_len);
				sub_len = sub_str.length();  // get real length
				if ((!done_f && (sub_len == 5)) &&  
					((sub_str == "+xus=") || 
					(sub_str == "+xsu=") ||
					(sub_str == "-xus=") ||
					(sub_str == "-xsu=") || 
					(sub_str == "*xus=") || 
					(sub_str == "*xsu="))) {
					code_wr_buf += " " + sub_str + " ";  // space ends
					i += sub_len;
					done_f = true;
				}
				sub_len = 4;  // 4 chars sub_str
				sub_str = code_rd_buf.substr(i, sub_len);
				sub_len = sub_str.length();  // get real length
				if ((!done_f && (sub_len == 4)) &&  
					((sub_str == "+us=") || 
					(sub_str == "+su=") ||
					(sub_str == "-us=") ||
					(sub_str == "-su=") ||
					(sub_str == "*us=") ||
					(sub_str == "*su=") ||
					(sub_str == "+xu=") ||
					(sub_str == "+xs=") ||
					(sub_str == "-xu=") ||
					(sub_str == "-xs=") ||
					(sub_str == "*xu=") ||
					(sub_str == "*xs=") ||
					(sub_str == "+xus") ||
					(sub_str == "+xsu") ||
					(sub_str == "-xus") ||
					(sub_str == "-xsu") ||
					(sub_str == "*xus") ||
					(sub_str == "*xsu") ||
					(sub_str == ":hu=") || 
					(sub_str == ":bu=") || 
					(sub_str == ":hs=") ||
					(sub_str == ":bs=") ||
					(sub_str == "<<r=") || 
					(sub_str == ">>r=") || 
					(sub_str == "<<<=") || 
					(sub_str == ">>>="))) {
					code_wr_buf += " " + sub_str + " ";  // space ends
					i += sub_len;
					done_f = true;
				}
				sub_len = 3;  // 3 chars sub_str
				sub_str = code_rd_buf.substr(i, sub_len);
				sub_len = sub_str.length();  // get real length
				if ((!done_f && (sub_len == 3)) &&  
					((sub_str == "+u=") || 
					(sub_str == "+s=") || 
					(sub_str == "-u=") || 
					(sub_str == "-s=") || 
					(sub_str == "*u=") || 
					(sub_str == "*s=") || 
					(sub_str == "+xu") || 
					(sub_str == "+xs") || 
					(sub_str == "-xu") || 
					(sub_str == "-xs") || 
					(sub_str == "*xu") || 
					(sub_str == "*xs") || 
					(sub_str == "+us") || 
					(sub_str == "+su") || 
					(sub_str == "-us") || 
					(sub_str == "-su") || 
					(sub_str == "*us") || 
					(sub_str == "*su") || 
					(sub_str == ":i=") || 
					(sub_str == ":h=") || 
					(sub_str == ":b=") || 
					(sub_str == "<<=") || 
					(sub_str == ">>=") || 
					(sub_str == "<<<") || 
					(sub_str == ">>>") ||
					(sub_str == "<<r") || 
					(sub_str == ">>r") ||
					(sub_str == "!<s") ||
					(sub_str == "!<u"))) { 
					code_wr_buf += " " + sub_str + " ";  // space ends
					i += sub_len;
					done_f = true;
				}
				sub_len = 2;  // 2 chars sub_str
				sub_str = code_rd_buf.substr(i, sub_len);
				sub_len = sub_str.length();  // get real length
				if ((!done_f && (sub_len == 2)) &&  
					((sub_str == "+=") || 
					(sub_str == "-=") || 
					(sub_str == "*=") || 
					(sub_str == "&=") || 
					(sub_str == "|=") || 
					(sub_str == "^=") || 
					(sub_str == ":=") || 
					(sub_str == "==") || 
					(sub_str == "!=") || 
					(sub_str == "!<") || 
					(sub_str == "--") || 
					(sub_str == "++") || 
					(sub_str == "<<") || 
					(sub_str == ">>") || 
					(sub_str == "+s") || 
					(sub_str == "-s") || 
					(sub_str == "*s") ||
					(sub_str == "<s") || 
					(sub_str == "+u") || 
					(sub_str == "-u") || 
					(sub_str == "*u") || 
					(sub_str == "<u"))) {
					code_wr_buf += " " + sub_str + " ";  // space ends
					i += sub_len;
					done_f = true;
				}
				sub_len = 1;  // 1 chars sub_str
				sub_str = code_rd_buf.substr(i, sub_len);
				sub_len = sub_str.length();  // get real length
				if ((!done_f && (sub_len == 1)) &&  
				   ((sub_str == ":") || 
				    (sub_str == "=") || 
					(sub_str == "(") || 
					(sub_str == ")") || 
					(sub_str == "[") || 
					(sub_str == "]") || 
					(sub_str == "{") || 
					(sub_str == "}") || 
					(sub_str == "<") || 
					(sub_str == ">") || 
					(sub_str == "!") || 
					(sub_str == "*") || 
					(sub_str == "&") || 
					(sub_str == "|") || 
					(sub_str == "^") || 
					(sub_str == "?") || 
					(sub_str == "~") || 
					(sub_str == ","))) { 
					code_wr_buf += " " + sub_str + " ";  // space both ends
					i += sub_len;
					done_f = true;
				}
				if ((!done_f && (sub_len == 1)) &&  
					((sub_str == "+") || 
					(sub_str == "-"))) { 
					code_wr_buf += " " + sub_str;  // space start only
					i += sub_len;
					done_f = true;
				}
				if (!done_f) {  // no match
					code_wr_buf += sub_str;  // just copy
					i++;  // next char
				}

			}  // end of space insertion

			// copy write buffer to output
			vsrc_io[idx_o].code = code_wr_buf;
	
		}  // end of input parsing
		
		if (error_f) { error_msg_o = "\nError PRE_3 : " + error_msg_o; }
		return (error_f);
	}



	// HAL pre-processing: 
	// implement simplistic forms of `define, `undef; 
	bool hal_pre_4(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
	    unordered_map<string, string> map;
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message

		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// declare & init stuff
			string code_buf;  // code buffer

			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
	
			// process `define
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (vtoken[0] == "`define") {
					if (vtoken.size() == 3) {  // 3 tokens
						if (vtoken[1][0] == '`') {  // error if `key
							error_f = true; 
							error_msg_o += "Bad `define key syntax : " + vtoken[1]; 
						}
						else if (vtoken[2][0] == '`') {  // error if `mapped
							error_f = true; 
							error_msg_o += "Bad `define map syntax : " + vtoken[2]; 
						}
						else if (map.count(vtoken[1])) {  // error if key exists
							error_f = true; 
							error_msg_o += "`define key already exists : " + vtoken[1]; 
						}
						else {  // ok
							map["`" + vtoken[1]] = vtoken[2];  // prepend ` to key and store
							vtoken.clear();  // kill all tokens
						}
					}
					else {  // bad param count
						error_f = true; 
						error_msg_o += "Bad `define parameter count";
					}
				}
				else if (vtoken[0] == "`undef") {
					if (vtoken.size() == 2) {  // 2 tokens
						string key = "`" + vtoken[1];
						if (!map.erase(key)) {  // try to erase
							error_f = true; 
							error_msg_o += "`undef key doesn't exist: " + key; 
						}
						vtoken.clear();  // kill all tokens
					}
					else {  // bad param count
						error_f = true; 
						error_msg_o += "Bad `undef parameter count";
					}
				}
				else if (vtoken[i][0] == '`') {
					if (map.count(vtoken[i])) {  // key exists
						vtoken[i] = map[vtoken[i]];  // replace token with map entry
					}
					else {  // key doesn't exist
						error_f = true; 
						error_msg_o += "`define key doesn't exist: " + vtoken[i]; 
					}
				}
			}

			// tokens to string, copy to I/O
			vsrc_io[idx_o].code = tokens_to_str(vtoken);
	
		}  // end of input parsing
		
		if (error_f) { error_msg_o = "\nError PRE_4 : " + error_msg_o; }
		return (error_f);
	}



	// HAL pre-processing: 
	// pre-process certain HAL constructs to make them easier to decode later
	bool hal_pre_5(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message

		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
	
			// delete commas
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (vtoken[i] == ",") { 
					vector<string>::iterator it = vtoken.begin() + i;
					vtoken.erase(it);
				}
			}
			// convert "sa := sb +/-im" to "sa := sb +/- im" (separate explicit sign as operation)
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 3) {  // 4 token min
					if (str_is_sab(vtoken[i-3]) && (vtoken[i-2] == ":=" ) && str_is_sab(vtoken[i-1]) && str_is_int(vtoken[i], "AUTO")) {
						if (vtoken[i][0] == '-') { 
							vtoken[i].erase(0,1);  // kill sign
							vector<string>::iterator it = vtoken.begin() + i;
							vtoken.insert(it, "-");  // insert sign
						}
						else if (vtoken[i][0] == '+') { 
							vtoken[i].erase(0,1);  // kill sign
							vector<string>::iterator it = vtoken.begin() + i;
							vtoken.insert(it, "+");  // insert sign
						}
						else { 
							error_f = true; 
							error_msg_o += "Immediate missing explicit sign (function)"; 
						}
					}
				}
			}
			// replace ++ with += 1
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (vtoken[i] == "++") { 
					vtoken[i] = "+="; 
					vector<string>::iterator it = vtoken.begin() + i;
					vtoken.insert(it+1, "1");
				}
			}
			// replace -- with -= 1
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (vtoken[i] == "--") { 
					vtoken[i] = "-="; 
					vector<string>::iterator it = vtoken.begin() + i;
					vtoken.insert(it+1, "1");
				}
			}
			// replace >>> im with <<< -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>>") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<<";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace >> im with << -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace >>r im with <<r -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>r") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<r";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace >>>= im with <<<= -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>>=") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<<=";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace >>= im with <<= -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>=") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<=";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace >>r= im with <<r= -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 1) { // 2 token min
					if ((vtoken[i-1] == ">>r=") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "<<r=";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}
			// replace pc -= im with pc += -im
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (i >= 2) { // 3 token min
					if ((vtoken[i-2] == "pc") && (vtoken[i-1] == "-=") && str_is_int(vtoken[i], "AUTO")) {
						int32_t neg_val = -str_to_int(vtoken[i], "AUTO");
						vtoken[i-1] = "+=";
						vtoken[i] = int_to_str(neg_val, "INT", ALU_W, true);
					}
				}
			}

			// tokens to string, copy to I/O
			vsrc_io[idx_o].code = tokens_to_str(vtoken);
	
		}  // end of input parsing
		
		if (error_f) { error_msg_o = "\nError PRE_5 : " + error_msg_o; }
		return (error_f);
	}

	
	
	// HAL processing, labels pass 0
	// 1. put explicit label address assignment values in map and kill assignment statement
	// 2. replace all (LH & RH) labels with mapped address values
	bool hal_lbl_0(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		unordered_map<string, uint32_t> map;
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		
		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
	
			// put explicitly assigned map values in map and kill explicit assignment expression
			if ((vtoken.size() == 3) && (vtoken[0][0] == '@') && (vtoken[1] == ":=") && str_is_int(vtoken[2], "AUTO")) {
				if (map.count(vtoken[0])) {  // error if label exists
					error_f = true; 
					error_msg_o += "Label already exists: " + vtoken[0]; 
				}
				else {
					map[vtoken[0]] = str_to_int(vtoken[2], "AUTO");  // store
					vtoken.clear();  // kill all tokens
				}
			}

			// tokens to string, copy to I/O
			vsrc_io[idx_o].code = tokens_to_str(vtoken);
	
		}  // end of input parsing

		if (!error_f) {

			// do until error or end of input is reached
			for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
		
				// declare & init stuff
				src_t src_buf;  // source buffer
	
				// tokenize
				vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
		
				// replace explicit label tokens with table values
				for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
					if (map.count(vtoken[i])) {
						vtoken[i] = int_to_str(map[vtoken[i]], "HEX", ALU_W, true);
					}
				}
		
				// tokens to string, copy to I/O
				vsrc_io[idx_o].code = tokens_to_str(vtoken);
		
			}  // end of input parsing
			
		}

		if (error_f) { error_msg_o = "\nError LBL_0 : " + error_msg_o; }
		return (error_f);
	}

	

	// HAL processing, labels pass 1: flatten label name space by traversing {scope} hierarchy and assigning globally unique aliases
	// 1. Put each LH label name in scope map, generate & associate unique alias, rename LH label with alias.
	// 2. Rename all RH lables with closest in scope LH alias.
	// All input LH labels at a given scoping level must be unique to that level.
	// Simple counter is used for globally unique label name generation.
	// A new label map is generated for each move deeper into the scoping heirarchy.
	bool hal_lbl_1(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o, uint32_t& labels_o, uint32_t& scopes_o) {
	
		// declare & init stuff
		unordered_map<string, string> empty_map;  // empty map to push
		vector<unordered_map<string, string>> vmap;  // map store
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		
		uint32_t lbl_num = 0;  // label number generator
		uint32_t map_num = 0;  // map number generator
		vector<int32_t> vmap_idx;  // maps index lifo
		
		vmap.push_back(empty_map);  // prime things
		vmap_idx.push_back(map_num);
		
		// do until error or end of input is reached
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
	
			// put LH key & alias in map, rename, do scoping
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if (!i && (vtoken[i][0] == '@')) {  // LH token
					if (vmap[vmap_idx.back()].count(vtoken[i])) {  // error if label exists
						error_f = true; 
						error_msg_o += "Label already exists: " + vtoken[i]; 
					}
					else {
						string alias = "@" + int_to_str(lbl_num, "UNS", ALU_W, false);  // form alias
						vmap[vmap_idx.back()][vtoken[i]] = alias;  // map
						vtoken[i] = alias;  // rename
						lbl_num++;  // generate new label number
					}
				}
				else if (vtoken[i] == "{") {  // going deeper into scope
					vmap.push_back( empty_map );  // push new empty map
					map_num++;  // generate new map number
					vmap_idx.push_back( map_num );  // push index
				}
				else if (vtoken[i] == "}") {  // backing out of scope
					if (vmap_idx.size() > 1) {
						vmap_idx.pop_back();  // pop index
					}
					else {  // error
						error_f = true; 
						error_msg_o += "Excess }"; 
					}
				}
			}

			// tokens to string, copy to I/O
			vsrc_io[idx_o].code = tokens_to_str(vtoken);
	
		}  // end of input parsing

		// check for bad scoping
		if (!error_f && (vmap_idx.size() > 1))  {
			error_f = true; 
			error_msg_o += "Excess {"; 
		}

		if (!error_f) {

			// init
			map_num = 0;
			
			// do until error or end of input is reached
			for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
		
				// tokenize
				vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
		
				// search line for tokens to replace with alias, do scoping
				for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
					if (i && (vtoken[i][0] == '@')) {  // RH token
						vector<int32_t> vmap_idx_tmp = vmap_idx;  // copy to temp
						bool match_f = false;
						while (!match_f && !vmap_idx_tmp.empty()) {  // check for key match
							if (vmap[vmap_idx_tmp.back()].count(vtoken[i])) {
								vtoken[i] = vmap[vmap_idx_tmp.back()][vtoken[i]];  // rename with alias
								match_f = true;  // done
							}
							else { vmap_idx_tmp.pop_back(); }  // keep looking
						}
						if (!match_f) {
							error_f = true; 
							error_msg_o += "No LH key match for RH label: " + vtoken[i]; 
						}
					}
					else if (vtoken[i] == "{") {  // going deeper into scope
						map_num++;  // generate new map number
						vmap_idx.push_back( map_num );  // push index
					}
					else if (vtoken[i] == "}") {  // backing out of scope
						vmap_idx.pop_back();  // pop index
					}
				}
		
				// tokens to string, copy to I/O
				vsrc_io[idx_o].code = tokens_to_str(vtoken);
		
			}  // end of input parsing
		}
		
		// output stats
		labels_o = lbl_num;
		scopes_o = map_num;

		if (error_f) { error_msg_o = "\nError LBL_1 : " + error_msg_o; }
		return (error_f);
	}


	// HAL processing, labels pass 2
	// kill scopes anchored by orphaned LH labels (recursively)
	// kill all braces.
	bool hal_lbl_2(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o, uint32_t& passes_o, uint32_t& dead_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		bool change_f = true;  // to enter while loop 1st time
		passes_o = 0;  // clear iteration count
		dead_o = 0;  // clear dead line count

		// iterative loop
		while (!error_f && change_f) {
		
			// declare & init stuff
			unordered_set<string> set;
			uint32_t scope = 0;

			// reset change flag
			change_f = false;
	
			// PHASE 0: put all RH labels in unordered set
			for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
		
				// tokenize
				vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
		
				// search line for RH labels
				for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
					if (i && vtoken[i][0] == '@') {  // RH label
						set.insert(vtoken[i]);  // put in set
					}
				}
			}  // PHASE 0 end
	

			// PHASE 1: kill code scopes anchored by orphaned LH labels
			if (!error_f) { 
				
				for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
			
					// declare & init stuff
					bool dead_f = false;  // reset flag
		
					// tokenize
					vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
			
					// search line 
					for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
						// if killing dead code
						if (scope) {
							if (vtoken[i] == "{") { scope++; }  // going deeper into scope
							if (vtoken[i] == "}") { scope--; }  // backing out of scope
							vtoken[i].clear();  // kill token
							dead_f = true;  // set flag
						}
						// else look for orphaned LH anchor label
						else if ((!i && vtoken[i][0] == '@') && !set.count(vtoken[i]) && (i+1 < vtoken.size()) && (vtoken[i+1] == "{")) { 
							scope = 1;  // start killing
							vtoken[i].clear();  // kill tokens
							vtoken[i+1].clear();
							dead_f = true;  // set flag
							change_f = true;  // set flag
						}
					}
					
					// inventory
					if (dead_f) { dead_o++; }
		
					// tokens to string, copy to I/O
					vsrc_io[idx_o].code = tokens_to_str(vtoken);
					
				}  

			}  // PHASE 1 end
			
			// update iteration count
			passes_o++;

		}  // done iterating


		// kill all {} braces
		for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
	
			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
	
			// search line for { } & delete
			for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
				if ((vtoken[i] == "{") || (vtoken[i] == "}")) {  
					vtoken[i].clear();
				}
			}

			// tokens to string, copy to I/O
			vsrc_io[idx_o].code = tokens_to_str(vtoken);
		
		}  // end of input parsing

		
		if (error_f) { error_msg_o = "\nError LBL_2 : " + error_msg_o; }
		return (error_f);
	}



	// HAL processing, labels pass 3: process implicitly assigned label tokens
	bool hal_lbl_3(vector<src_t>& vsrc_io, string& error_msg_o, uint32_t& idx_o, uint32_t& passes_o) {
	
		// declare & init stuff
		unordered_map<string, uint32_t> map;
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		idx_o = 0;  // clear idx_o
	
		// declare & init iterative stuff
		uint32_t wr_addr = 0;
		bool change_f = true;  // to enter while loop 1st time
		passes_o = 0;  // clear iteration count
	
		// iterative loop, do until no changes (relaxation)
		while (!error_f && change_f) {
		
			// reset change flag
			change_f = false;
	
			// clear address
			wr_addr = 0;
			
			// do until error or end of input is reached
			for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
		
				// tokenize
				vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
		
				// if number at start of line: set address to number and remove it
				if (!error_f && vtoken.size() && str_is_int(vtoken[0], "AUTO")) {
					wr_addr = str_to_int(vtoken[0], "AUTO");
					if (wr_addr > (MEM_DEPTH-1)) { 
						error_f = true; 
						error_msg_o += "Out-of-bounds addr: " + vtoken[0]; 
					}
					vector<string>::iterator it = vtoken.begin();
					vtoken.erase(it);
					if (!error_f && !vtoken.size()) {
						error_f = true; 
						error_msg_o += "Naked address assignment"; 
					}
				}
	
				// create, update, and replace remaining implicit labels with map values
				for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
					if (vtoken[i][0] == '@') {
					
						// declare helper flags
						bool sol_f = !i;  // at start of line
						bool it0_f = !passes_o;  // initial pass
						bool def_f = map.count(vtoken[i]);  // label exists
						uint32_t lbl_addr = wr_addr;  // replacement value default
	
						// LH labels
						if (sol_f && it0_f && !def_f) {  // LH label is not defined at 1st pass
							map[vtoken[i]] = wr_addr;  // create & assign LH label
							change_f = true;  // do another pass
						}
						else if (sol_f && it0_f && def_f) {  // LH label is defined at 1st pass
							error_f = true;  // found a dup
							error_msg_o += "Duplicate LH label: " + vtoken[i]; 
						}
						else if (sol_f && !it0_f && !def_f) {  // LH lable is not defined after 1st pass
							error_f = true;  // shouldn't ever happen
							error_msg_o += "Undefined LH label: " + vtoken[i]; 
						}
						else if (sol_f && !it0_f && def_f) {  // LH label is defined after 1st pass
							if (map[vtoken[i]] != wr_addr) {  // address mismatch?
								map[vtoken[i]] = wr_addr;  // update address
								change_f = true;  // do another pass
							}
						}
						// other labels
						else if (!sol_f && it0_f && !def_f) {  // RH label is not defined at 1st pass
							change_f = true;  // do another pass
						}
						else if (!sol_f && !it0_f && !def_f) {  // RH label is not defined after 1st pass
							error_f = true;  // found an orphan
							error_msg_o += "Undefined RH label: " + vtoken[i]; 
						}
						else {  // (!sol_f && def_f)  // RH label is defined
							lbl_addr = map[vtoken[i]];  // use label addr
						}
						
						// update label token
						vtoken[i] = int_to_str(lbl_addr, "HEX", ALU_W, true);
	
					}
				}
	
				// calculate next address
				if (!error_f) { 
					vector<uint8_t> oc;
					int32_t lits = 0;
					error_f = hal_to_oc(vtoken, wr_addr, oc, lits, error_msg_o);
					wr_addr += oc.size();
				}
	
			}  // end of input parsing

			// update iteration count
			passes_o++;
			if (passes_o > LBL_PASS_MAX) { error_f = true; error_msg_o += "Label passes exceeded limit: " + int_to_str(passes_o, "UNS", ALU_W, true); }
	
		}  // end of iterating
	
		// remove remaining labels
		if (!error_f) { 
			wr_addr = 0;  // clear address
	
			// do until error or end of input is reached
			for (idx_o = 0; !error_f && (idx_o < vsrc_io.size()); idx_o++) {  
		
				// tokenize
				vector<string> vtoken = str_to_tokens(vsrc_io[idx_o].code);
		
				// all remaining labels are implicit: kill beginning of line labels, replace all others with table values
				for (uint32_t i = 0; !error_f && (i < vtoken.size()); i++) { 
					if (vtoken[i][0] == '@') {  // label
						if (!i) {  // start of line
							vtoken[i].clear();  // clear label
						}
						else {  // not start of line, replace with table value
							vtoken[i] = int_to_str(map[vtoken[i]], "HEX", ALU_W, true);
						}
					}
				}		

				// tokens to string, copy to I/O
				vsrc_io[idx_o].code = tokens_to_str(vtoken);
		
			}  // end of input parsing
		}

		if (error_f) { error_msg_o = "\nError LBL_3 : " + error_msg_o; }
		return (error_f);
	}
	
	
	// HAL processing: write parsed assembly to memory array
	bool hal_to_mem(const vector<src_t>& vsrc_i, string& error_msg_o, uint32_t& idx_o) {
	
		// declare & init stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
		idx_o = 0;  // clear idx_o
		uint32_t wr_addr = 0;  // clear write address
		z();  // zero out memory
	
		// do until error or end of input is reached
		for (uint32_t idx_o = 0; !error_f && (idx_o < vsrc_i.size()); idx_o++) {  
	
			// tokenize
			vector<string> vtoken = str_to_tokens(vsrc_i[idx_o].code);
	
			// set address to number at start of line (if present) and remove it
			if (!error_f && vtoken.size() && str_is_int(vtoken[0], "AUTO")) {
				wr_addr = str_to_int(vtoken[0], "AUTO");
				wr_addr &= MEM_ADDR_MSK;
				vector<string>::iterator it = vtoken.begin();
				vtoken.erase(it);
			}
	
			// convert hal to opcode and lit bytes, write to mem array, write comments to mem
			if (!error_f) { 
				// declare stuff
				vector<uint8_t> oc;
				int32_t lits = 0;
				// get oc/lit bytes, lit count, errors, comment
				error_f = hal_to_oc(vtoken, wr_addr, oc, lits, error_msg_o);
				// process oc/lit bytes	
				for (uint32_t i = 0; !error_f && (i < oc.size()); i++) {
					error_f = use_f[wr_addr];
					if (error_f) { error_msg_o += "Opcode collision addr: " + int_to_str(wr_addr, "HEX", MEM_ADDR_W, true); }  // already used check
					else {
						wr(wr_addr, 1, oc[i]);  // write byte
						use_f[wr_addr] = true;  // mark used
						if (!i) { eolc[wr_addr] = vsrc_i[idx_o].eolc; }  // @ start, write comment
						if (lits) {  // lit processing
							lit_f[wr_addr] = true;  // mark lits
							int32_t lit_bytes = oc.size() / lits;  // calc bytes per lit
							if (!(i % lit_bytes)) {  // @ start of lit
								bytes[wr_addr] = lit_bytes; // write lit size
								file[wr_addr] = vsrc_i[idx_o].file; // write file name
								line[wr_addr] = vsrc_i[idx_o].line; // write line number
							}  
						}
						else if (!i) {  // not lit @ start
							bytes[wr_addr] = oc.size(); // write byte count
							file[wr_addr] = vsrc_i[idx_o].file; // write file name
							line[wr_addr] = vsrc_i[idx_o].line; // write line number
						}
						wr_addr++;
						wr_addr &= MEM_ADDR_MSK;
					}
				}
			}
	
		}  // end of input parsing
	
		if (error_f) { error_msg_o = "\nError MEM : " + error_msg_o; }
		return (error_f);
	}
	
	
	// write mif config files
	bool mem_to_mif(bool compress_i, string& error_msg_o) { 
	
		// declare stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	    vector<string> filename = { "hive_0.mif", "hive_1.mif", "hive_2.mif", "hive_3.mif" };
		ofstream mif_file;  // file stream
		const int32_t BANKS = 4;
		const uint32_t BANK_DEPTH = MEM_DEPTH / BANKS;
	
	
		// iterate thru banks & files
		for(int32_t bank = 0; bank < BANKS; bank++) {
	
			// open file
			mif_file.open(filename[bank].c_str());
	
			// check for open files
			if (!mif_file.is_open()) { error_f = true; error_msg_o += "Unable to open file: " + filename[bank]; }
			
			if (!error_f) { 
				// file header
				mif_file << "WIDTH = " << int_to_str(BYT_W, "UNS", ALU_W, false) << ";" << endl;
				mif_file << "DEPTH = " << int_to_str(BANK_DEPTH, "UNS", ALU_W, false) << ";" << endl;
				mif_file << "ADDRESS_RADIX = HEX;" << endl;
				mif_file << "DATA_RADIX = HEX;" << endl;
				mif_file << "CONTENT BEGIN" << endl;
	
				// declare stuff
				uint8_t data;
				uint32_t addr, addr_end;
	
				// file body
				for(addr = 0; addr < BANK_DEPTH; addr++) {
					addr_end = addr;
					data = mem[((addr << 2) + bank)];
					// compress identical data
					while ((addr_end+1 < BANK_DEPTH) && (mem[(((addr_end + 1) << 2) + bank)] == data) && compress_i) {
						addr_end++;
					}
					// do compressed line
					if (addr != addr_end) {
						mif_file << "[" << int_to_str(addr, "HEX", ALU_W, false);
						mif_file << ".." << int_to_str(addr_end, "HEX", ALU_W, false);
						mif_file << "] : " << int_to_str(data, "HEX", ALU_W, false) << ";" << endl;
						addr = addr_end;  // update addr
					}
					// do normal line
					else {
						mif_file << int_to_str(addr, "HEX", ALU_W, false) << " : ";
						mif_file << int_to_str(data, "HEX", ALU_W, false) << ";" << endl;
					}
				}
	
				mif_file << "END;" << endl;
				// done with file
				mif_file.close();
			}
		}
	
		if (error_f) { error_msg_o = "\nMIF Error : " + error_msg_o; }
		return (error_f); 
	}


	// write ttl config file, report assembly stats
	bool mem_to_ttl(string& error_msg_o) { 
	
		// declare stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	    string tempfilename = "temp.ttl";
		ofstream ttl_file;  // file stream
		uint32_t mem_addr = 0;  // mem addr
		uint32_t spi_addr = SPI_OFFS;  // spi addr
		uint32_t spi_writes = 0;  // spi write count
		crc = -1;  // crc init

		// open file
		ttl_file.open(tempfilename.c_str());

		// check for open files
		if (!ttl_file.is_open()) { error_f = true; error_msg_o += "Unable to open file: " + tempfilename; }
		
		if (!error_f) { 
			// file header
			ttl_file << "; Tera Term macro" << endl;
			ttl_file << "; Hive SPI image" << endl;
			ttl_file << endl;
			ttl_file << "dly_erase = 1000" << endl;
			ttl_file << "dly_pg_wr = 6" << endl;
			ttl_file << endl;

			// disable all XSRs
			ttl_file << "; Disable XSRs:" << endl;
			ttl_file << "send \'0 0xff00 wr \'" << endl;  // xsr[7:0] off
			ttl_file << "wait \'>\'" << endl;
			ttl_file << "mpause dly_pg_wr" << endl;  // delay
			ttl_file << endl;

			// initial reg CS
			ttl_file << "; Initial reg CS:" << endl;
			ttl_file << "send \'6 0x100 wr \'" << endl;  // spi_cs
			ttl_file << "wait \'>\'" << endl;
			
			// file body
			ttl_file << "; Process mem data:" << endl;
			while (spi_writes < SPI_LENGTH) {
				if (!(spi_addr % 256)) {  // @ page end: program & next page
					ttl_file << "send \'0x100 wr \'" << endl;  // spi_cs
					ttl_file << "wait \'>\'" << endl;
					ttl_file << "mpause dly_pg_wr" << endl;  // delay
					ttl_file << "send \'0x06 wr \'" << endl;  // cmd: wr_en
					ttl_file << "wait \'>\'" << endl;
				}
				if (!spi_writes) {  // @ start, store length
					crc = crc_32(crc ^ SPI_LENGTH);  // do crc
					ttl_file << "send \'" << int_to_str(spi_addr, "HEX", 32, true) << " " << int_to_str(SPI_LENGTH, "HEX", 32, true) << " ws \'" << endl;  // length
					ttl_file << "wait \'>\'" << endl;
					spi_writes += 4;
					spi_addr += 4;
				}
				else if (spi_writes < (SPI_LENGTH - 4)) {  // in body, store data
					crc = crc_32(crc ^ rd(mem_addr, 4));  // do crc
					if (!(spi_addr % 256)) {  // @ page end
						ttl_file << "send \'" << int_to_str(spi_addr, "HEX", 32, true) << " " << int_to_str(rd(mem_addr, 4), "HEX", 32, true) << " ws \'" << endl;  // data
					}
					else {
						ttl_file << "send \'" << int_to_str(rd(mem_addr, 4), "HEX", 32, true) << " ws \'" << endl;  // data
					}
					ttl_file << "wait \'>\'" << endl;
					spi_writes += 4;
					spi_addr += 4;
					mem_addr += 4;
				}
				else {  // @ end, store crc
					crc = ~crc;  // invert residue to get crc
					if (!(spi_addr % 256)) {  // @ page end
						ttl_file << "send \'" << int_to_str(spi_addr, "HEX", 32, true) << " " << int_to_str(crc, "HEX", 32, true) << " ws \'" << endl;  // crc
					}
					else {
						ttl_file << "send \'" << int_to_str(crc, "HEX", 32, true) << " ws \'" << endl;  // crc
					}
					ttl_file << "wait \'>\'" << endl;
					spi_writes += 4;
					spi_addr += 4;
				}
			}
			// final chip select
			ttl_file << "send \'0x100 wr \'" << endl;  // spi_cs
			ttl_file << "wait \'>\'" << endl;
			ttl_file << "mpause dly_pg_wr" << endl;  // delay
			ttl_file << endl;

			// calculate & report flashed load CRC32
			ttl_file << "; Calculate flashed CRC32:" << endl;
			ttl_file << "send \'" << "crc \'" << endl;  // crc
			ttl_file << "wait \'>\'" << endl;
			ttl_file << endl;

			// reset all threads
			ttl_file << "; Reset all threads:" << endl;
			ttl_file << "send \'0 0xff000000 wr \'" << endl;  // rst[7:0]
			ttl_file << endl;

			// info
			ttl_file << "; INFO: bytes = " << "0x" << hex << spi_writes << " (" << dec << spi_writes << ")" << endl;
			ttl_file << "; INFO: CRC32 = " << int_to_str(crc, "HEX", 32, true) << endl;
			ttl_file << "; INFO: DONE!" << endl;
			ttl_file.close();
		}
		if (!error_f) { 
		    string crcfilename = int_to_str(crc, "HEX", 32, false) + ".ttl";  // new file name based on crc
		    remove(crcfilename.c_str());  // kill it if it exists
			error_f = rename(tempfilename.c_str(), crcfilename.c_str());  // rename
			if (error_f) { error_msg_o += "Unable to rename TTL file"; }
		}
		
		if (error_f) { error_msg_o = "\nTTL Error : " + error_msg_o; }
		return (error_f); 
	}


	// write spi config file, report assembly stats
	bool mem_to_spi(string& error_msg_o) { 
	
		// declare stuff
		bool error_f = false;  // clear error flag
		error_msg_o.clear();  // clear error message
	    string tempfilename = "temp.spi";
		ofstream spi_file;  // file stream
		uint32_t mem_addr = 0;  // mem addr
		uint32_t spi_writes = 0;  // spi write count
		crc = -1;  // crc init

		// open file
		spi_file.open(tempfilename.c_str());

		// check for open files
		if (!spi_file.is_open()) { error_f = true; error_msg_o += "Unable to open file: " + tempfilename; }
		
		if (!error_f) { 
			while (spi_writes < SPI_LENGTH) {
				if (!spi_writes) {  // @ start, store length
					crc = crc_32(crc ^ SPI_LENGTH);  // do crc
					spi_file << int_to_str(SPI_LENGTH, "HEX", 32, false) << endl;  // length
					spi_writes += 4;
				}
				else if (spi_writes < (SPI_LENGTH - 4)) {  // in body, store data
					crc = crc_32(crc ^ rd(mem_addr, 4));  // do crc
					spi_file << int_to_str(rd(mem_addr, 4), "HEX", 32, false) << endl;  // data
					spi_writes += 4;
					mem_addr += 4;
				}
				else {  // @ end, store crc
					crc = ~crc;  // invert residue to get crc
					spi_file << int_to_str(crc, "HEX", 32, false);  // crc
					spi_writes += 4;
				}
			}
			spi_file.close();
		}
		if (!error_f) { 
		    string crcfilename = int_to_str(crc, "HEX", 32, false) + ".spi";  // new file name based on crc
		    remove(crcfilename.c_str());  // kill it if it exists
			error_f = rename(tempfilename.c_str(), crcfilename.c_str());  // rename
			if (error_f) { error_msg_o += "Unable to rename SPI file"; }
		}
		
		if (error_f) { error_msg_o = "\nSPI Error : " + error_msg_o; }
		return (error_f); 
	}


	// debug output to file
	void dbg_vsrc(const vector<src_t>& vsrc_i) {
		ofstream dbg_file;  
		dbg_file.open("dbg.txt");
		for (uint32_t i = 0; i < vsrc_i.size(); i++) {  
			dbg_file << endl << i << "  " << vsrc_i[i].file << "  " << vsrc_i[i].line << "  " << vsrc_i[i].code;
			if (vsrc_i[i].eolc.size()) { dbg_file << "  // " << vsrc_i[i].eolc; }
		}
		dbg_file.close();
	}

	
	// parse HAL file
	bool hal_parse(const string& filename_i, bool quiet_i) {
	
		struct timespec start, finish;  // timing stuff
		double elapsed;
		uint32_t files = 0;  // files processed
		uint32_t lines = 0;  // lines processed
		uint32_t scopes = 0;  // scopes processed
		uint32_t dupes = 0;  // duplicate `include files (ignored)
		uint32_t dead_lines = 0;  // dead lines (deleted)
		uint32_t dead_passes = 0;  // dead iterations
		uint32_t labels = 0;  // labels processed
		uint32_t lbl_passes = 0;  // label iterations
		bool error_f = false;  // error flag
		string error_file = "";  // error file
		string error_msg = "";  // error message
	    vector<src_t> vsrc;  // source container
		uint32_t idx = 0;

    	// start timing
		clock_gettime(CLOCK_MONOTONIC, &start);
		
		if(!quiet_i) { 
			cout << "\n **********************";
			cout << "\n * HIVE HAL Assembler *";
			cout << "\n **********************";
			cout << endl;
			cout << "\n Input file    : " << filename_i;
			// run the gauntlet
			if(!error_f) { error_f = hal_pre_0(vsrc, error_msg, idx, files, dupes, filename_i); lines = vsrc.size(); }
			// dbg_vsrc(vsrc);  // debug out
			if(!error_f) { error_f = hal_pre_1(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_pre_2(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_pre_3(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_pre_4(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_pre_5(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_lbl_0(vsrc, error_msg, idx); }
			if(!error_f) { error_f = hal_lbl_1(vsrc, error_msg, idx, labels, scopes); }
			if(!error_f) { error_f = hal_lbl_2(vsrc, error_msg, idx, dead_passes, dead_lines); }
			if(!error_f) { error_f = hal_lbl_3(vsrc, error_msg, idx, lbl_passes); }
			if(!error_f) { error_f = hal_to_mem(vsrc, error_msg, idx); }
			// adjust idx (generally incremented to next)
			if (idx) { idx--; }
			// report first error
			if (error_f) { 
				cout << endl << error_msg; 
				if (idx < vsrc.size()) {
					cout << "\nError file  : " << vsrc[idx].file; 
					cout << "\nError line  : " << vsrc[idx].line; 
					cout << "\nError code  : " << vsrc[idx].code; 
					cout << "\nError eolc  : " << vsrc[idx].eolc; 
				}
				cout << endl; 
			}
			else { 
				// declare stuff & calc stats
				uint32_t op_count = 0;
				uint32_t lit_count = 0;
				uint32_t op_bytes = 0;
				uint32_t lit_bytes = 0;
				for(uint32_t addr = 0; addr < MEM_DEPTH; addr++) {
					if (bytes[addr]) {
						if (lit_f[addr]) { lit_count++; lit_bytes += bytes[addr]; }
						else { op_count++; op_bytes += bytes[addr]; }
					}
				}
				uint32_t mem_bytes = op_bytes + lit_bytes;
				// output stats
				cout << endl; 
				cout << "\n Files         : " << files;
				cout << "\n Lines         : " << lines;
				cout << "\n Labels        : " << labels;
				cout << "\n Scopes        : " << scopes;
				cout << endl; 
				cout << "\n Dupe files    : " << dupes;
				cout << "\n Dead lines    : " << dead_lines;
				cout << endl; 
				cout << "\n MEM Bytes     : " << mem_bytes; 
				cout << "\n MEM %         : " << (100.0 * mem_bytes) / float(MEM_DEPTH); 
				cout << endl; 
				cout << "\n OP %          : " << (100.0 * op_bytes) / float(mem_bytes); 
				cout << "\n LIT %         : " << (100.0 * lit_bytes) / float(mem_bytes); 
				cout << endl; 
				cout << "\n Dead passes   : " << dead_passes; 
				cout << "\n Label passes  : " << lbl_passes; 
				string mif_msg = "";
				cout << "\n MIF write     : ";
				if (mem_to_mif(true, mif_msg)) { cout << mif_msg << endl; }
				else { cout << "OK"; }
/*
				string ttl_msg = "";
				cout << "\n TTL write     : ";
				if (mem_to_ttl(ttl_msg)) { cout << ttl_msg << endl; }
				else { cout << "OK"; }
*/
				string spi_msg = "";
				cout << "\n SPI write     : ";
				if (mem_to_spi(spi_msg)) { cout << spi_msg << endl; }
				else { cout << "OK"; }

				// output crc
				cout << "\n CRC32         : " << int_to_str(crc, "HEX", 32, true);
				cout << endl; 
			}
			// version
			cout << "\n Core Version  : " << int_to_str(VER, "HEX", 16, true);
			// stop, calc, & report timing
			clock_gettime(CLOCK_MONOTONIC, &finish);
			elapsed = (finish.tv_sec - start.tv_sec);
			elapsed += (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
			cout << "\n Elapsed time  : " << elapsed << "s\n";
			// announce exit
			cout << "\n *********************";
			cout << "\n * Exiting Assembler *";
			cout << "\n *********************";
			cout << "\n\n (set console to " << CON_W << "x" << CON_H << ")";
			cout << "\n (type 'help' to display help file)";
		}
		return (error_f);
	}
	
	
	// decode HAL string given memory address (disassemble)
	string addr_to_hal(uint32_t addr_i, const string& radix_i) {
	
		// if unused or not at start of opcode
		if ( !use_f[addr_i] || !bytes[addr_i] ) { return("-"); }
	
		// get opcode
		int64_t oc = rd(addr_i, bytes[addr_i]);
		
		// oc subsets
		int32_t  im32  = oc >> 16;
		uint16_t im16u = im32;
		int16_t  im16  = im32;
		uint8_t  im8u  = im32;
		int8_t   im8   = im32;
		//
		int32_t im24_lo = oc >> 8;
		int16_t im16_lo = im24_lo;
		uint8_t im8u_lo = im24_lo;
		int8_t  im8_lo  = im24_lo;
	
		// default string
		string hal = "????????";  
		
		// literal value
		if (lit_f[addr_i]) { 
			if (bytes[addr_i] == 1) { hal = "mem[1] :b= "; }
			else if (bytes[addr_i] == 2) { hal = "mem[1] :h= "; }
			else { hal = "mem[1] := "; }
			hal += int_to_str(oc, radix_i, ALU_W, true); 
		}
		else {
			// mask lower byte
			switch (oc & 0xFF) { 
				case OP_NOP : hal = "nop"; break;
				case OP_HLT : hal = "hlt"; break;
				//
				case OP_POP : 
					hal = "";
					for (int32_t i = 7; i >= 0; i--) {
						if ((im8u_lo >> i) & 1) { hal += "P" + int_to_str(i, "OCT", 3, false) + " "; }
					}
				break;
				case OP_CLS : 
					hal = "";
					for (int32_t i = 7; i >= 0; i--) {
						if ((im8u_lo >> i) & 1) { hal += "C" + int_to_str(i, "OCT", 3, false) + " "; }
					}
				break;
				//
				case OP_JMP_8 :
					if (im8_lo < 0) { hal = "pc -= "; hal += int_to_str(-im8_lo, radix_i, BYT_W, true); }
					else { hal = "pc += "; hal += int_to_str(im8_lo, radix_i, BYT_W, true); }
				break;
				case OP_JMP_16 :
					if (im16_lo < 0) { hal = "pc -= "; hal += int_to_str(-im16_lo, radix_i, HLF_W, true); }
					else { hal = "pc += "; hal += int_to_str(im16_lo, radix_i, HLF_W, true); }
				break;
				case OP_JMP_24 :
					if (im24_lo < 0) { hal = "pc -= "; hal += int_to_str(-im24_lo, radix_i, 24, true); }
					else { hal = "pc += "; hal += int_to_str(im24_lo, radix_i, 24, true); }
				break;
				//
				case OP_CPY     : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc); break;
				case OP_NSB     : hal = oc_sa_str(oc) + " := nsb(" + oc_sb_str(oc) + ")"; break;
				case OP_LIM     : hal = oc_sa_str(oc) + " := lim(" + oc_sb_str(oc) + ")"; break;
				case OP_SAT     : hal = oc_sa_str(oc) + " := sat(" + oc_sb_str(oc) + ")"; break;
				case OP_FLP     : hal = oc_sa_str(oc) + " := flp(" + oc_sb_str(oc) + ")"; break;
				case OP_SWP     : hal = oc_sa_str(oc) + " := swp(" + oc_sb_str(oc) + ")"; break;
				case OP_NOT     : hal = oc_sa_str(oc) + " := ~" + oc_sb_str(oc); break;
				case OP_BRX     : hal = oc_sa_str(oc) + " := ^" + oc_sb_str(oc); break;
				case OP_SGN     : hal = oc_sa_str(oc) + " := sgn(" + oc_sb_str(oc) + ")"; break;
				case OP_LZC     : hal = oc_sa_str(oc) + " := lzc(" + oc_sb_str(oc) + ")"; break;
				case OP_AND     : hal = oc_sa_str(oc) + " &= " + oc_sb_str(oc); break;
				case OP_ORR     : hal = oc_sa_str(oc) + " |= " + oc_sb_str(oc); break;
				case OP_XOR     : hal = oc_sa_str(oc) + " ^= " + oc_sb_str(oc); break;
				//
				case OP_JMP_Z   : hal = "(" + oc_sa_str(oc) + " == 0) ? pc += " + oc_sb_str(oc); break;
				case OP_JMP_NZ  : hal = "(" + oc_sa_str(oc) + " != 0) ? pc += " + oc_sb_str(oc); break;
				case OP_JMP_LZ  : hal = "(" + oc_sa_str(oc) + " < 0) ? pc += " + oc_sb_str(oc); break;
				case OP_JMP_NLZ : hal = "(" + oc_sa_str(oc) + " !< 0) ? pc += " + oc_sb_str(oc); break;
				case OP_JMP     : hal = "pc += " + oc_sb_str(oc) + oc_pa_str(oc); break;
				case OP_GTO     : hal = "pc := " + oc_sb_str(oc) + oc_pa_str(oc); break;
				case OP_IRT     : hal = "pc :i= " + oc_sb_str(oc) + oc_pa_str(oc); break;
				case OP_JSB     : hal = oc_sa_str(oc) + " := pc += " + oc_sb_str(oc); break;
				case OP_GSB     : hal = oc_sa_str(oc) + " := pc := " + oc_sb_str(oc); break;
				case OP_PCR     : hal = oc_sa_str(oc) + " := pc" + oc_pb_str(oc); break;
				//
				case OP_ADD_U   : hal = oc_sa_str(oc) + " +u= " + oc_sb_str(oc); break;
				case OP_ADD_US  : hal = oc_sa_str(oc) + " +us= " + oc_sb_str(oc); break;
				case OP_ADD_SU  : hal = oc_sa_str(oc) + " +su= " + oc_sb_str(oc); break;
				case OP_ADD_S   : hal = oc_sa_str(oc) + " +s= " + oc_sb_str(oc); break;
				case OP_SUB_U   : hal = oc_sa_str(oc) + " -u= " + oc_sb_str(oc); break;
				case OP_SUB_US  : hal = oc_sa_str(oc) + " -us= " + oc_sb_str(oc); break;
				case OP_SUB_SU  : hal = oc_sa_str(oc) + " -su= " + oc_sb_str(oc); break;
				case OP_SUB_S   : hal = oc_sa_str(oc) + " -s= " + oc_sb_str(oc); break;
				case OP_SBR_U   : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -u " + oc_sa_str(oc); break;
				case OP_SBR_US  : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -us " + oc_sa_str(oc); break;
				case OP_SBR_SU  : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -su " + oc_sa_str(oc); break;
				case OP_SBR_S   : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -s " + oc_sa_str(oc); break;
				case OP_MUL_U   : hal = oc_sa_str(oc) + " *u= " + oc_sb_str(oc); break;
				case OP_MUL_US  : hal = oc_sa_str(oc) + " *us= " + oc_sb_str(oc); break;
				case OP_MUL_SU  : hal = oc_sa_str(oc) + " *su= " + oc_sb_str(oc); break;
				case OP_MUL_S   : hal = oc_sa_str(oc) + " *s= " + oc_sb_str(oc); break;
				case OP_SHL_U   : hal = oc_sa_str(oc) + " <<= " + oc_sb_str(oc); break;
				case OP_SHL_S   : hal = oc_sa_str(oc) + " <<<= " + oc_sb_str(oc); break;
				case OP_ROL     : hal = oc_sa_str(oc) + " <<r= " + oc_sb_str(oc); break;
				case OP_POW     : hal = oc_sa_str(oc) + " := 1 << " + oc_sb_str(oc); break;
				//
				case OP_ADD_XU  : hal = oc_sa_str(oc) + " +xu= " + oc_sb_str(oc); break;
				case OP_ADD_XUS : hal = oc_sa_str(oc) + " +xus= " + oc_sb_str(oc); break;
				case OP_ADD_XSU : hal = oc_sa_str(oc) + " +xsu= " + oc_sb_str(oc); break;
				case OP_ADD_XS  : hal = oc_sa_str(oc) + " +xs= " + oc_sb_str(oc); break;
				case OP_SUB_XU  : hal = oc_sa_str(oc) + " -xu= " + oc_sb_str(oc); break;
				case OP_SUB_XUS : hal = oc_sa_str(oc) + " -xus= " + oc_sb_str(oc); break;
				case OP_SUB_XSU : hal = oc_sa_str(oc) + " -xsu= " + oc_sb_str(oc); break;
				case OP_SUB_XS  : hal = oc_sa_str(oc) + " -xs= " + oc_sb_str(oc); break;
				case OP_SBR_XU  : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -xu " + oc_sa_str(oc); break;
				case OP_SBR_XUS : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -xus " + oc_sa_str(oc); break;
				case OP_SBR_XSU : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -xsu " + oc_sa_str(oc); break;
				case OP_SBR_XS  : hal = oc_sa_str(oc) + " := " + oc_sb_str(oc) + " -xs " + oc_sa_str(oc); break;
				case OP_MUL_XU  : hal = oc_sa_str(oc) + " *xu= " + oc_sb_str(oc); break;
				case OP_MUL_XUS : hal = oc_sa_str(oc) + " *xus= " + oc_sb_str(oc); break;
				case OP_MUL_XSU : hal = oc_sa_str(oc) + " *xsu= " + oc_sb_str(oc); break;
				case OP_MUL_XS  : hal = oc_sa_str(oc) + " *xs= " + oc_sb_str(oc); break;
				//
				case OP_MEM_R   : hal = oc_sa_str(oc) + " := mem[" + oc_sb_str(oc) + "]"; break;
				case OP_MEM_W   : hal = "mem[" + oc_sb_str(oc) + "] := " + oc_sa_str(oc); break;
				case OP_MEM_WH  : hal = "mem[" + oc_sb_str(oc) + "] :h= " + oc_sa_str(oc); break;
				case OP_MEM_WB  : hal = "mem[" + oc_sb_str(oc) + "] :b= " + oc_sa_str(oc); break;
				case OP_MEM_RHS : hal = oc_sa_str(oc) + " :hs= mem[" + oc_sb_str(oc) + "]"; break;
				case OP_MEM_RHU : hal = oc_sa_str(oc) + " :hu= mem[" + oc_sb_str(oc) + "]"; break;
				case OP_MEM_RBS : hal = oc_sa_str(oc) + " :bs= mem[" + oc_sb_str(oc) + "]"; break;
				case OP_MEM_RBU : hal = oc_sa_str(oc) + " :bu= mem[" + oc_sb_str(oc) + "]"; break;
				case OP_LIT     : hal = oc_sa_str(oc) + " := " + int_to_str(im32, radix_i, ALU_W, true) + oc_pb_str(oc); break;
				case OP_LIT_HS  : hal = oc_sa_str(oc) + " :hs= " + int_to_str(im16, radix_i, HLF_W, true) + oc_pb_str(oc);	break;
				case OP_LIT_HU  : hal = oc_sa_str(oc) + " :hu= " + int_to_str(im16u, radix_i, HLF_W, true) + oc_pb_str(oc); break;
				case OP_LIT_BS  : hal = oc_sa_str(oc) + " :bs= " + int_to_str(im8, radix_i, BYT_W, true) + oc_pb_str(oc); break;
				case OP_LIT_BU  : hal = oc_sa_str(oc) + " :bu= " + int_to_str(im8u, radix_i, BYT_W, true) + oc_pb_str(oc); break;
				//
				case OP_REG_R   : hal = oc_sa_str(oc) + " := reg[" + oc_sb_str(oc) + "]"; break;
				case OP_REG_W   : hal = "reg[" + oc_sb_str(oc) + "] := " + oc_sa_str(oc); break;
				//
				case OP_JMP_8Z   :
				case OP_JMP_8NZ  :
				case OP_JMP_8LZ  :
				case OP_JMP_8NLZ :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_8Z   : hal += " == "; break;
						case OP_JMP_8NZ  : hal += " != "; break;
						case OP_JMP_8LZ  : hal += " < "; break;
						case OP_JMP_8NLZ : hal += " !< "; break;
					}
					hal += "0) ? pc";
					if (im8 < 0) { hal += " -= " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " += " + int_to_str(im8, radix_i, BYT_W, true); }
					hal += oc_pb_str(oc); 
				break; 
				//
				case OP_JMP_8O   :
				case OP_JMP_8NO  :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_8O   : hal += " == "; break;
						case OP_JMP_8NO  : hal += " != "; break;
					}
					hal += "odd) ? pc";
					if (im8 < 0) { hal += " -= " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " += " + int_to_str(im8, radix_i, BYT_W, true); }
					hal += oc_pb_str(oc); 
				break; 
				//
				case OP_JMP_8E   :
				case OP_JMP_8NE  :
				case OP_JMP_8LS  :
				case OP_JMP_8NLS :
				case OP_JMP_8LU  :
				case OP_JMP_8NLU :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_8E   : hal += " == "; break;
						case OP_JMP_8NE  : hal += " != "; break;
						case OP_JMP_8LS  : hal += " <s "; break;
						case OP_JMP_8NLS : hal += " !<s "; break;
						case OP_JMP_8LU  : hal += " <u "; break;
						case OP_JMP_8NLU : hal += " !<u "; break;
					}
					hal += oc_sb_str(oc) + ") ? pc";
					if (im8 < 0) { hal += " -= " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " += " + int_to_str(im8, radix_i, BYT_W, true); }
				break; 
				case OP_JSB_8  : 
					hal = oc_sa_str(oc) + " := pc";
					if (im8 < 0) { hal += " -= " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " += " + int_to_str(im8, radix_i, BYT_W, true); }
					hal += oc_pb_str(oc); 
				break;
				//
				case OP_ADD_8U  :
				case OP_ADD_8US :
				case OP_ADD_8SU :
				case OP_ADD_8S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						if (((oc & 0xFF) == OP_ADD_8U) && im8 == 1) { hal += "++"; }
						else {
							switch (oc & 0xFF) { 
								case OP_ADD_8U  : hal += " +u= "; break;
								case OP_ADD_8US : hal += " +us= "; break;
								case OP_ADD_8SU : hal += " +su= "; break;
								case OP_ADD_8S  : hal += " +s= "; break;
							}
							hal += int_to_str(im8, radix_i, BYT_W, true); 
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_ADD_8U  : hal += " +u "; break;
							case OP_ADD_8US : hal += " +us "; break;
							case OP_ADD_8SU : hal += " +su "; break;
							case OP_ADD_8S  : hal += " +s "; break;
						}
						hal += int_to_str(im8, radix_i, BYT_W, true); 
					}
				break;
				case OP_SUB_8U  :
				case OP_SUB_8US :
				case OP_SUB_8SU :
				case OP_SUB_8S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						if (((oc & 0xFF) == OP_SUB_8U) && im8 == 1) { hal += "--"; }
						else {
							switch (oc & 0xFF) { 
								case OP_SUB_8U  : hal += " -u= "; break;
								case OP_SUB_8US : hal += " -us= "; break;
								case OP_SUB_8SU : hal += " -su= "; break;
								case OP_SUB_8S  : hal += " -s= "; break;
							}
							hal += int_to_str(im8, radix_i, BYT_W, true); 
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_SUB_8U  : hal += " -u "; break;
							case OP_SUB_8US : hal += " -us "; break;
							case OP_SUB_8SU : hal += " -su "; break;
							case OP_SUB_8S  : hal += " -s "; break;
						}
						hal += int_to_str(im8, radix_i, BYT_W, true); 
					}
				break;
				case OP_SBR_8U  :
				case OP_SBR_8US :
				case OP_SBR_8SU :
				case OP_SBR_8S  :
					hal = oc_sa_str(oc) + " := " + int_to_str(im8, radix_i, BYT_W, true);
					switch (oc & 0xFF) { 
						case OP_SBR_8U  : hal += " -u "; break;
						case OP_SBR_8US : hal += " -us "; break;
						case OP_SBR_8SU : hal += " -su "; break;
						case OP_SBR_8S  : hal += " -s "; break;
					}
					hal += oc_sb_str(oc); 
				break;
				case OP_MUL_8U  :
				case OP_MUL_8US :
				case OP_MUL_8SU :
				case OP_MUL_8S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_MUL_8U  : hal += " *u= "; break;
							case OP_MUL_8US : hal += " *us= "; break;
							case OP_MUL_8SU : hal += " *su= "; break;
							case OP_MUL_8S  : hal += " *s= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_MUL_8U  : hal += " *u "; break;
							case OP_MUL_8US : hal += " *us "; break;
							case OP_MUL_8SU : hal += " *su "; break;
							case OP_MUL_8S  : hal += " *s "; break;
						}
					}
					hal += int_to_str(im8, radix_i, BYT_W, true); 
				break;
				case OP_SHL_8S  : 
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						if (im8 < 0) { hal += " >>>= " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " <<<= " + int_to_str(im8, radix_i, BYT_W, true); }
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						if (im8 < 0) { hal += " >>> " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " <<< " + int_to_str(im8, radix_i, BYT_W, true); }
					}
				break;
				case OP_SHL_8U :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						if (im8 < 0) { hal += " >>= " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " <<= " + int_to_str(im8, radix_i, BYT_W, true); }
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						if (im8 < 0) { hal += " >> " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " << " + int_to_str(im8, radix_i, BYT_W, true); }
					}
				break;
				case OP_ROL_8 :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						if (im8 < 0) { hal += " >>r= " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " <<r= " + int_to_str(im8, radix_i, BYT_W, true); }
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						if (im8 < 0) { hal += " >>r " + int_to_str(-im8, radix_i, BYT_W, true); }
						else { hal += " <<r " + int_to_str(im8, radix_i, BYT_W, true); }
					}
				break;
				case OP_POW_8  : hal = oc_sa_str(oc) + " := 1 << " + int_to_str(im8u, radix_i, BYT_W, true) + oc_pb_str(oc); break;
				//
				case OP_MEM_8R    : hal = oc_sa_str(oc) + " := mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "]"; break;
				case OP_MEM_8W    : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "] := " + oc_sa_str(oc); break;
				case OP_MEM_8WH   : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "] :h= " + oc_sa_str(oc); break;
				case OP_MEM_8WB   : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "] :b= " + oc_sa_str(oc); break;
				case OP_MEM_8RHS  : hal = oc_sa_str(oc) + " :hs= mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "]"; break;
				case OP_MEM_8RHU  : hal = oc_sa_str(oc) + " :hu= mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "]"; break;
				case OP_MEM_8RBS  : hal = oc_sa_str(oc) + " :bs= mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "]"; break;
				case OP_MEM_8RBU  : hal = oc_sa_str(oc) + " :bu= mem[" + oc_sb_str(oc) + "+" + int_to_str(im8u, radix_i, BYT_W, true) + "]"; break;
				case OP_MEM_I8R   : 
				case OP_MEM_I8RHS : 
				case OP_MEM_I8RHU : 
				case OP_MEM_I8RBS : 
				case OP_MEM_I8RBU : 
					hal = oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_MEM_I8R   : hal += " := "; break;
						case OP_MEM_I8RHS : hal += " :hs= "; break;
						case OP_MEM_I8RHU : hal += " :hu= "; break;
						case OP_MEM_I8RBS : hal += " :bs= "; break;
						case OP_MEM_I8RBU : hal += " :bu= "; break;
					}
					hal += "mem[pc";
					if (im8 < 0) { hal += " - " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " + " + int_to_str(im8, radix_i, BYT_W, true); }
					hal += "]" + oc_pb_str(oc); 
				break;
				case OP_MEM_I8W   :
				case OP_MEM_I8WH  :
				case OP_MEM_I8WB  :
					hal = "mem[pc";
					if (im8 < 0) { hal += " - " + int_to_str(-im8, radix_i, BYT_W, true); }
					else { hal += " + " + int_to_str(im8, radix_i, BYT_W, true); }
					hal += "]"; 
					switch (oc & 0xFF) { 
						case OP_MEM_I8W   : hal += " := "; break;
						case OP_MEM_I8WH  : hal += " :h= "; break;
						case OP_MEM_I8WB  : hal += " :b= "; break;
					}
					hal += oc_sa_str(oc) + oc_pb_str(oc); 
				break;
				//
				case OP_AND_8 :
				case OP_ORR_8 :
				case OP_XOR_8 :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_AND_8  : hal += " &= "; break;
							case OP_ORR_8  : hal += " |= "; break;
							case OP_XOR_8  : hal += " ^= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_AND_8  : hal += " & "; break;
							case OP_ORR_8  : hal += " | "; break;
							case OP_XOR_8  : hal += " ^ "; break;
						}
					}
					hal += int_to_str(im8, radix_i, BYT_W, true);
				break;
				//
				case OP_REG_8R   : hal = oc_sa_str(oc) + " := reg[" + int_to_str(im8u, radix_i, BYT_W, true) + "]" + oc_pb_str(oc); break;
				case OP_REG_8W   : hal = "reg[" + int_to_str(im8u, radix_i, BYT_W, true) + "] := " + oc_sa_str(oc) + oc_pb_str(oc); break;
				//
				case OP_JMP_16Z   :
				case OP_JMP_16NZ  :
				case OP_JMP_16LZ  :
				case OP_JMP_16NLZ :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_16Z   : hal += " == "; break;
						case OP_JMP_16NZ  : hal += " != "; break;
						case OP_JMP_16LZ  : hal += " < "; break;
						case OP_JMP_16NLZ : hal += " !< "; break;
					}
					hal += "0) ? pc";
					if (im16 < 0) { hal += " -= " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " += " + int_to_str(im16, radix_i, HLF_W, true); }
					hal += oc_pb_str(oc); 
				break; 
				//
				case OP_JMP_16O   :
				case OP_JMP_16NO  :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_16O   : hal += " == "; break;
						case OP_JMP_16NO  : hal += " != "; break;
					}
					hal += "odd) ? pc";
					if (im16 < 0) { hal += " -= " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " += " + int_to_str(im16, radix_i, HLF_W, true); }
					hal += oc_pb_str(oc); 
				break; 
				//
				case OP_JMP_16E   :
				case OP_JMP_16NE  :
				case OP_JMP_16LS  :
				case OP_JMP_16NLS :
				case OP_JMP_16LU  :
				case OP_JMP_16NLU :
					hal = "(" + oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_JMP_16E   : hal += " == "; break;
						case OP_JMP_16NE  : hal += " != "; break;
						case OP_JMP_16LS  : hal += " <s "; break;
						case OP_JMP_16NLS : hal += " !<s "; break;
						case OP_JMP_16LU  : hal += " <u "; break;
						case OP_JMP_16NLU : hal += " !<u "; break;
					}
					hal += oc_sb_str(oc) + ") ? pc";
					if (im16 < 0) { hal += " -= " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " += " + int_to_str(im16, radix_i, HLF_W, true); }
				break; 
				case OP_JSB_16  : 
					hal = oc_sa_str(oc) + " := pc";
					if (im16 < 0) { hal += " -= " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " += " + int_to_str(im16, radix_i, HLF_W, true); }
					hal += oc_pb_str(oc); 
				break;
				//
				case OP_ADD_16U  :
				case OP_ADD_16US  :
				case OP_ADD_16SU  :
				case OP_ADD_16S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_ADD_16U  : hal += " +u= "; break;
							case OP_ADD_16US : hal += " +us= "; break;
							case OP_ADD_16SU : hal += " +su= "; break;
							case OP_ADD_16S  : hal += " +s= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_ADD_16U  : hal += " +u "; break;
							case OP_ADD_16US : hal += " +us "; break;
							case OP_ADD_16SU : hal += " +su "; break;
							case OP_ADD_16S  : hal += " +s "; break;
						}
					}
					hal += int_to_str(im16, radix_i, HLF_W, true); 
				break;
				case OP_SUB_16U  :
				case OP_SUB_16US :
				case OP_SUB_16SU :
				case OP_SUB_16S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_SUB_16U  : hal += " -u= "; break;
							case OP_SUB_16US : hal += " -us= "; break;
							case OP_SUB_16SU : hal += " -su= "; break;
							case OP_SUB_16S  : hal += " -s= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_SUB_16U  : hal += " -u "; break;
							case OP_SUB_16US : hal += " -us "; break;
							case OP_SUB_16SU : hal += " -su "; break;
							case OP_SUB_16S  : hal += " -s "; break;
						}
					}
					hal += int_to_str(im16, radix_i, HLF_W, true); 
				break;
				case OP_SBR_16U  :
				case OP_SBR_16US :
				case OP_SBR_16SU :
				case OP_SBR_16S  :
					hal = oc_sa_str(oc) + " := " + int_to_str(im16, radix_i, HLF_W, true);
					switch (oc & 0xFF) { 
						case OP_SBR_16U  : hal += " -u "; break;
						case OP_SBR_16US : hal += " -us "; break;
						case OP_SBR_16SU : hal += " -su "; break;
						case OP_SBR_16S  : hal += " -s "; break;
					}
					hal += oc_sb_str(oc); 
				break;
				case OP_MUL_16U  :
				case OP_MUL_16US :
				case OP_MUL_16SU :
				case OP_MUL_16S  :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_MUL_16U  : hal += " *u= "; break;
							case OP_MUL_16US : hal += " *us= "; break;
							case OP_MUL_16SU : hal += " *su= "; break;
							case OP_MUL_16S  : hal += " *s= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_MUL_16U  : hal += " *u "; break;
							case OP_MUL_16US : hal += " *us "; break;
							case OP_MUL_16SU : hal += " *su "; break;
							case OP_MUL_16S  : hal += " *s "; break;
						}
					}
					hal += int_to_str(im16, radix_i, HLF_W, true); 
				break;
				//
				case OP_MEM_16R    : hal = oc_sa_str(oc) + " := mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "]"; break;
				case OP_MEM_16W    : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "] := " + oc_sa_str(oc); break;
				case OP_MEM_16WH   : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "] :h= " + oc_sa_str(oc); break;
				case OP_MEM_16WB   : hal = "mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "] :b= " + oc_sa_str(oc); break;
				case OP_MEM_16RHS  : hal = oc_sa_str(oc) + " :hs= mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "]"; break;
				case OP_MEM_16RHU  : hal = oc_sa_str(oc) + " :hu= mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "]"; break;
				case OP_MEM_16RBS  : hal = oc_sa_str(oc) + " :bs= mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "]"; break;
				case OP_MEM_16RBU  : hal = oc_sa_str(oc) + " :bu= mem[" + oc_sb_str(oc) + "+" + int_to_str(im16u, radix_i, HLF_W, true) + "]"; break;
				case OP_MEM_I16R   : 
				case OP_MEM_I16RHS : 
				case OP_MEM_I16RHU : 
				case OP_MEM_I16RBS : 
				case OP_MEM_I16RBU : 
					hal = oc_sa_str(oc);
					switch (oc & 0xFF) { 
						case OP_MEM_I16R   : hal += " := "; break;
						case OP_MEM_I16RHS : hal += " :hs= "; break;
						case OP_MEM_I16RHU : hal += " :hu= "; break;
						case OP_MEM_I16RBS : hal += " :bs= "; break;
						case OP_MEM_I16RBU : hal += " :bu= "; break;
					}
					hal += "mem[pc";
					if (im16 < 0) { hal += " - " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " + " + int_to_str(im16, radix_i, HLF_W, true); }
					hal += "]" + oc_pb_str(oc); 
				break;
				case OP_MEM_I16W   :
				case OP_MEM_I16WH  :
				case OP_MEM_I16WB  :
					hal = "mem[pc";
					if (im16 < 0) { hal += " - " + int_to_str(-im16, radix_i, HLF_W, true); }
					else { hal += " + " + int_to_str(im16, radix_i, HLF_W, true); }
					hal += "]"; 
					switch (oc & 0xFF) { 
						case OP_MEM_I16W   : hal += " := "; break;
						case OP_MEM_I16WH  : hal += " :h= "; break;
						case OP_MEM_I16WB  : hal += " :b= "; break;
					}
					hal += oc_sa_str(oc) + oc_pb_str(oc); 
				break;
				//
				case OP_AND_16 :
				case OP_ORR_16 :
				case OP_XOR_16 :
					if (oc_sa(oc) == oc_sb(oc)) {
						if (oc_pa(oc)) { hal = oc_sa_str(oc); }
						else { hal = oc_sb_str(oc); }
						switch (oc & 0xFF) { 
							case OP_AND_16  : hal += " &= "; break;
							case OP_ORR_16  : hal += " |= "; break;
							case OP_XOR_16  : hal += " ^= "; break;
						}
					}
					else {
						hal = oc_sa_str(oc) + " := " + oc_sb_str(oc);
						switch (oc & 0xFF) { 
							case OP_AND_16  : hal += " & "; break;
							case OP_ORR_16  : hal += " | "; break;
							case OP_XOR_16  : hal += " ^ "; break;
						}
					}
					hal += int_to_str(im16, radix_i, HLF_W, true);
				break;
			}
		}
		return (hal);
	};
	
	
	// calculate offset address given address and lines to offset
	uint32_t calc_addr(uint32_t addr_i, int32_t lines_i) {
		uint32_t addr = addr_i;
		int32_t line = 0;
		if (lines_i < 0) {
			do {
				addr--;
				addr &= MEM_ADDR_MSK;  // modulo
				if( !use_f[addr] || bytes[addr] ) { line--; };
			} while ((line > lines_i) || (use_f[addr] && !bytes[addr]));
			return( addr );
		}
		else {
			while ((line < lines_i) || (use_f[addr] && !bytes[addr])) {
				addr++;
				addr &= MEM_ADDR_MSK;  // modulo
				if( !use_f[addr] || bytes[addr] ) { line++; };
			}
			return( addr );
		}
	}


	// list lines of disassembly vertically centered on input address
	void draw(short x_i, short y_i, uint32_t addr_i, uint32_t pc_i, const string& radix_i) {

		// constants
		const int32_t ADDR_W = 8;
		const int32_t BYT_W = 6;
		const int32_t LIT_W = 4;
		const int32_t OC_W = 16;
		const int32_t LINE_W = 8;
		const int32_t HAL_W = 32;
		const int32_t EOLC_W = 50;
		const int32_t LINES = 33;
	
		// draw header
		con_xy(x_i, y_i);
		con_font('i');
		cout << setw(ADDR_W) << right << "ADDR";
		cout << setw(BYT_W) << right <<  "BYT";
		cout << setw(LIT_W) << right <<  "LIT";
		cout << setw(OC_W) << right <<  "OC";
		cout << setw(LINE_W) << right <<  "LINE";
		cout << "  ";
		cout << setw(HAL_W) << left <<  "HAL";
		cout << setw(2+EOLC_W) << left << "EOLC";

		// get start address
		uint32_t addr = calc_addr(addr_i, -16);

		// draw listing
		con_font('n');
		for (uint32_t ln = 0; ln < LINES; ln++) {
			con_xy(x_i, y_i + 1 + ln);
			if (addr == pc_i) { con_font('o'); } // change font
			else if (ln == LINES/2) { con_font('h'); }
			else { con_font('n'); }  // normal font
			// process in-use slot
			if (use_f[addr]) {
				cout << setw(ADDR_W) << right << hex << showbase << addr;  // note: showbase
				cout << setw(BYT_W) << right << dec << bytes[addr];
				cout << setw(LIT_W) << right << (lit_f[addr] ? "#" : ".");
				cout << setw(OC_W) << right << hex << rd(addr, bytes[addr]);
				cout << setw(LINE_W) << right << dec << line[addr];
				cout << "  ";
				cout << setw(HAL_W) << left << addr_to_hal(addr, radix_i);
				if (!eolc[addr].empty()) { cout << left << "// " << setw(EOLC_W) << eolc[addr]; }
				else { cout << left << "  " << setw(EOLC_W) << " "; }
				addr += bytes[addr];  // inc address
				addr &= MEM_ADDR_MSK;  // modulo
			}
			// process unused slot
			else {
				cout << setw(ADDR_W) << right << hex << showbase << addr;  // note: showbase
				cout << setw(BYT_W) << right << dec << 1;
				cout << setw(LIT_W) << right << ".";
				cout << setw(OC_W) << right << ".";
				cout << setw(LINE_W) << right << ".";
				cout << "  ";
				cout << setw(HAL_W) << left << ".";
				cout << left << "  " << setw(EOLC_W) << " ";
				addr++;  // inc address
				addr &= MEM_ADDR_MSK;  // modulo
			}
		}
	}
};

