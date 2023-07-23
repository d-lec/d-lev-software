// hive_data_ring.cpp
#pragma once


// return operation cls
bitset<STACKS> op_cls(bool clt_i, bool irq_i, uint32_t oc_i) {
	bitset<STACKS> cls = 0;  // default is no clear
	if (clt_i) {  // clear thread
		cls = 0xFF;  // clear all stacks
	}
	else if (!irq_i) {  // not clt or irq
		switch (oc_i & 0xFF) { 
			case OP_CLS   : cls = oc_i >> 8; break;
		}
	}
	return(cls);
}


// return operation pop
bitset<STACKS> op_pop(bool clt_i, bool irq_i, uint32_t oc_i) {
	bitset<STACKS> pop = 0;  // default is no pop
	if (!clt_i && !irq_i) {  // not clt or irq
		switch (oc_i & 0xFF) { 
			case OP_NOP    :
			case OP_HLT    :
			case OP_JMP_8  :
			case OP_JMP_16 :
			case OP_JMP_24 : break;
			case OP_POP    : pop = oc_i >> 8; break;
			default : 
				pop = ((oc_pa(oc_i) << oc_sa(oc_i)) | (oc_pb(oc_i) << oc_sb(oc_i)));
			break;
		}
	}
	return(pop);
}


// return operation push
bitset<STACKS> op_psh(bool clt_i, bool irq_i, uint32_t oc_i) {
	bitset<STACKS> psh = 0;  // default is no push
	if (!clt_i) {  // not clt
		if (irq_i) {  // irq, push pc to stack 7
			psh = 1 << 7;
		}
		else {  // not irq
			switch (oc_i & 0xFF) { 
				case OP_CPY       :
				case OP_NSB       :
				case OP_LIM       :
				case OP_SAT       :
				case OP_FLP       :
				case OP_SWP       :
				case OP_NOT       :
				case OP_BRX       :
				case OP_SGN       :
				case OP_LZC       :
				case OP_AND       :
				case OP_ORR       :
				case OP_XOR       :
				//
				case OP_JSB       :
				case OP_GSB       :
				case OP_PCR       :
				//
				case OP_ADD_U     :
				case OP_ADD_US    :
				case OP_ADD_SU    :
				case OP_ADD_S     :
				case OP_SUB_U     :
				case OP_SUB_US    :
				case OP_SUB_SU    :
				case OP_SUB_S     :
				case OP_SBR_U     :
				case OP_SBR_US    :
				case OP_SBR_SU    :
				case OP_SBR_S     :
				case OP_MUL_U     :
				case OP_MUL_US    :
				case OP_MUL_SU    :
				case OP_MUL_S     :
				case OP_SHL_U     :
				case OP_SHL_S     :
				case OP_ROL       :
				case OP_POW       :
				//
				case OP_ADD_XU    :
				case OP_ADD_XUS   :
				case OP_ADD_XSU   :
				case OP_ADD_XS    :
				case OP_SUB_XU    :
				case OP_SUB_XUS   :
				case OP_SUB_XSU   :
				case OP_SUB_XS    :
				case OP_SBR_XU    :
				case OP_SBR_XUS   :
				case OP_SBR_XSU   :
				case OP_SBR_XS    :
				case OP_MUL_XU    :
				case OP_MUL_XUS   :
				case OP_MUL_XSU   :
				case OP_MUL_XS    :
				//
				case OP_MEM_R     :
				case OP_MEM_RHS   :
				case OP_MEM_RHU   :
				case OP_MEM_RBS   :
				case OP_MEM_RBU   :
				case OP_LIT       :
				case OP_LIT_HS    :
				case OP_LIT_HU    :
				case OP_LIT_BS    :
				case OP_LIT_BU    :
				//
				case OP_REG_R     :
				//
				case OP_JSB_8     :
				//
				case OP_ADD_8U    :
				case OP_ADD_8US   :
				case OP_ADD_8SU   :
				case OP_ADD_8S    :
				case OP_SUB_8U    :
				case OP_SUB_8US   :
				case OP_SUB_8SU   :
				case OP_SUB_8S    :
				case OP_SBR_8U    :
				case OP_SBR_8US   :
				case OP_SBR_8SU   :
				case OP_SBR_8S    :
				case OP_MUL_8U    :
				case OP_MUL_8US   :
				case OP_MUL_8SU   :
				case OP_MUL_8S    :
				case OP_SHL_8U    :
				case OP_SHL_8S    : 
				case OP_ROL_8     : 
				case OP_POW_8     :
				//
				case OP_MEM_8R    :
				case OP_MEM_8RHS  :
				case OP_MEM_8RHU  :
				case OP_MEM_8RBS  :
				case OP_MEM_8RBU  :
				case OP_MEM_I8R   :
				case OP_MEM_I8RHS :
				case OP_MEM_I8RHU :
				case OP_MEM_I8RBS :
				case OP_MEM_I8RBU :
				//
				case OP_REG_8R    :
				//
				case OP_AND_8     :
				case OP_ORR_8     :
				case OP_XOR_8     :
				//
				case OP_JSB_16    :
				//
				case OP_ADD_16U   :
				case OP_ADD_16US  :
				case OP_ADD_16SU  :
				case OP_ADD_16S   :
				case OP_SUB_16U   :
				case OP_SUB_16US  :
				case OP_SUB_16SU  :
				case OP_SUB_16S   :
				case OP_SBR_16U   :
				case OP_SBR_16US  :
				case OP_SBR_16SU  :
				case OP_SBR_16S   :
				case OP_MUL_16U   : 
				case OP_MUL_16US  :
				case OP_MUL_16SU  : 
				case OP_MUL_16S   :
				//
				case OP_MEM_16R    :
				case OP_MEM_16RHS  :
				case OP_MEM_16RHU  :
				case OP_MEM_16RBS  :
				case OP_MEM_16RBU  :
				case OP_MEM_I16R   :
				case OP_MEM_I16RHS :
				case OP_MEM_I16RHU :
				case OP_MEM_I16RBS :
				case OP_MEM_I16RBU :
				//
				case OP_AND_16     :
				case OP_ORR_16     :
				case OP_XOR_16     : psh[oc_sa(oc_i)] = 1; break;
			}
		}
	}
	return(psh);
}


// clock
void data_ring_clk(bool clk_phase_i) {

	//////////////////
	// static state //
	//////////////////
	static alu_t a_async[THREADS];
	static alu_t b_async[THREADS];
	static bitset<THREADS> pop_er_async;
	static bitset<THREADS> psh_er_async;
	static bitset<STACKS> pop_async_1;
	static bitset<STACKS> cls_async_0;
	static bitset<STACKS> psh_async_5;
	static uint8_t id_async_0, id_async_1, id_async_5, id_async_7;
	static uint32_t pcr_async_3;
	static uint32_t oc_async_3;
	static bool clt_async_3, irq_async_3;

	///////////
	// async //
	///////////
	if (!clk_phase_i) {
		// default data copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			a_async[thd] = a[thd];
			b_async[thd] = b[thd];
			//
			pop_er_async[thd] = pop_er[thd];
			psh_er_async[thd] = psh_er[thd];
		}
		oc_async_3 = oc[3];
		clt_async_3 = clt[3];
		irq_async_3 = irq[3];
		id_async_0 = id[0];
		id_async_1 = id[1];
		id_async_5 = id[5];
		id_async_7 = id[7];
		/////////////
		// stage 0 //
		/////////////
		// A & B stacks read, inject into ring
		a_async[0] = stacks[id[0]].top(oc_sa(oc[0]));
		b_async[0] = stacks[id[0]].top(oc_sb(oc[0]));
		//
		cls_async_0 = op_cls(clt[0], irq[0], oc[0]);  // thread clear
		/////////////
		// stage 1 //
		/////////////
		pop_async_1 = op_pop(clt[1], irq[1], oc[1]);  // pop
		/////////////
		// stage 3 //
		/////////////
		pcr_async_3 = pcr_3;
		if (irq[3]) {  // irq: push pc
			a_async[3] = to_alu_t(pcr_3); 
		}
		else {  // not irq
			int8_t im8 = oc[3] >> 16;
			int16_t im16 = oc[3] >> 16;
			switch (oc[3] & 0xFF) { 
				case OP_CPY       : a_async[3] = b[3]; break;
				case OP_NSB       : a_async[3] = alu_nsb(b[3]); break;
				case OP_LIM       : a_async[3] = alu_lim(b[3]); break;
				case OP_SAT       : a_async[3] = alu_sat(b[3]); break;
				case OP_FLP       : a_async[3] = alu_flp(b[3]); break;
				case OP_SWP       : a_async[3] = alu_swp(b[3]); break;
				case OP_NOT       : a_async[3] = alu_not(b[3]); break;
				case OP_BRX       : a_async[3] = alu_brx(b[3]); break;
				case OP_SGN       : a_async[3] = alu_sgn(b[3]); break;
				case OP_LZC       : a_async[3] = alu_lzc(b[3]); break;
				case OP_AND       : a_async[3] = alu_and(a[3], b[3]); break;
				case OP_ORR       : a_async[3] = alu_orr(a[3], b[3]); break;
				case OP_XOR       : a_async[3] = alu_xor(a[3], b[3]); break;
				//
				case OP_JSB       : a_async[3] = to_alu_t(pcr_3); break;
				case OP_JSB_8     : a_async[3] = to_alu_t(pcr_3); break;
				case OP_JSB_16    : a_async[3] = to_alu_t(pcr_3); break;
				case OP_GSB       : a_async[3] = to_alu_t(pcr_3); break;
				case OP_PCR       : a_async[3] = to_alu_t(pcr_3); break;
				//
				case OP_ADD_U     : a_async[3] = alu_add(a[3], b[3], 0, 0, 0); break;
				case OP_ADD_US    : a_async[3] = alu_add(a[3], b[3], 0, 1, 0); break;
				case OP_ADD_SU    : a_async[3] = alu_add(a[3], b[3], 1, 0, 0); break;
				case OP_ADD_S     : a_async[3] = alu_add(a[3], b[3], 1, 1, 0); break;
				case OP_SUB_U     : a_async[3] = alu_sub(a[3], b[3], 0, 0, 0); break;
				case OP_SUB_US    : a_async[3] = alu_sub(a[3], b[3], 0, 1, 0); break;
				case OP_SUB_SU    : a_async[3] = alu_sub(a[3], b[3], 1, 0, 0); break;
				case OP_SUB_S     : a_async[3] = alu_sub(a[3], b[3], 1, 1, 0); break;
				case OP_SBR_U     : a_async[3] = alu_sbr(a[3], b[3], 0, 0, 0); break;
				case OP_SBR_US    : a_async[3] = alu_sbr(a[3], b[3], 0, 1, 0); break;
				case OP_SBR_SU    : a_async[3] = alu_sbr(a[3], b[3], 1, 0, 0); break;
				case OP_SBR_S     : a_async[3] = alu_sbr(a[3], b[3], 1, 1, 0); break;
				case OP_MUL_U     : a_async[3] = alu_mul(a[3], b[3], 0, 0, 0); break;
				case OP_MUL_US    : a_async[3] = alu_mul(a[3], b[3], 0, 1, 0); break;
				case OP_MUL_SU    : a_async[3] = alu_mul(a[3], b[3], 1, 0, 0); break;
				case OP_MUL_S     : a_async[3] = alu_mul(a[3], b[3], 1, 1, 0); break;
				case OP_SHL_U     : a_async[3] = alu_shl(a[3], b[3], 0); break;
				case OP_SHL_S     : a_async[3] = alu_shl(a[3], b[3], 1); break;
				case OP_ROL       : a_async[3] = alu_rol(a[3], b[3]); break;
				case OP_POW       : a_async[3] = alu_pow(b[3]); break;
				//
				case OP_ADD_XU    : a_async[3] = alu_add(a[3], b[3], 0, 0, 1); break;
				case OP_ADD_XUS   : a_async[3] = alu_add(a[3], b[3], 0, 1, 1); break;
				case OP_ADD_XSU   : a_async[3] = alu_add(a[3], b[3], 1, 0, 1); break;
				case OP_ADD_XS    : a_async[3] = alu_add(a[3], b[3], 1, 1, 1); break;
				case OP_SUB_XU    : a_async[3] = alu_sub(a[3], b[3], 0, 0, 1); break;
				case OP_SUB_XUS   : a_async[3] = alu_sub(a[3], b[3], 0, 1, 1); break;
				case OP_SUB_XSU   : a_async[3] = alu_sub(a[3], b[3], 1, 0, 1); break;
				case OP_SUB_XS    : a_async[3] = alu_sub(a[3], b[3], 1, 1, 1); break;
				case OP_SBR_XU    : a_async[3] = alu_sbr(a[3], b[3], 0, 0, 1); break;
				case OP_SBR_XUS   : a_async[3] = alu_sbr(a[3], b[3], 0, 1, 1); break;
				case OP_SBR_XSU   : a_async[3] = alu_sbr(a[3], b[3], 1, 0, 1); break;
				case OP_SBR_XS    : a_async[3] = alu_sbr(a[3], b[3], 1, 1, 1); break;
				case OP_MUL_XU    : a_async[3] = alu_mul(a[3], b[3], 0, 0, 1); break;
				case OP_MUL_XUS   : a_async[3] = alu_mul(a[3], b[3], 0, 1, 1); break;
				case OP_MUL_XSU   : a_async[3] = alu_mul(a[3], b[3], 1, 0, 1); break;
				case OP_MUL_XS    : a_async[3] = alu_mul(a[3], b[3], 1, 1, 1); break;
				//
				case OP_REG_R     :
				case OP_REG_8R    : a_async[3] = to_alu_t(rbus_rd_data_3); break;
				//
				case OP_AND_8     : a_async[3] = alu_and(b[3], to_alu_t(im8)); break;
				case OP_ORR_8     : a_async[3] = alu_orr(b[3], to_alu_t(im8)); break;
				case OP_XOR_8     : a_async[3] = alu_xor(b[3], to_alu_t(im8)); break;
				//
				case OP_ADD_8U    : a_async[3] = alu_add(b[3], to_alu_t(im8), 0, 0, 0); break;
				case OP_ADD_8US   : a_async[3] = alu_add(b[3], to_alu_t(im8), 0, 1, 0); break;
				case OP_ADD_8SU   : a_async[3] = alu_add(b[3], to_alu_t(im8), 1, 0, 0); break;
				case OP_ADD_8S    : a_async[3] = alu_add(b[3], to_alu_t(im8), 1, 1, 0); break;
				case OP_SUB_8U    : a_async[3] = alu_sub(b[3], to_alu_t(im8), 0, 0, 0); break;
				case OP_SUB_8US   : a_async[3] = alu_sub(b[3], to_alu_t(im8), 0, 1, 0); break;
				case OP_SUB_8SU   : a_async[3] = alu_sub(b[3], to_alu_t(im8), 1, 0, 0); break;
				case OP_SUB_8S    : a_async[3] = alu_sub(b[3], to_alu_t(im8), 1, 1, 0); break;
				case OP_SBR_8U    : a_async[3] = alu_sbr(b[3], to_alu_t(im8), 0, 0, 0); break;
				case OP_SBR_8US   : a_async[3] = alu_sbr(b[3], to_alu_t(im8), 0, 1, 0); break;
				case OP_SBR_8SU   : a_async[3] = alu_sbr(b[3], to_alu_t(im8), 1, 0, 0); break;
				case OP_SBR_8S    : a_async[3] = alu_sbr(b[3], to_alu_t(im8), 1, 1, 0); break;
				case OP_MUL_8U    : a_async[3] = alu_mul(b[3], to_alu_t(im8), 0, 0, 0); break;
				case OP_MUL_8US   : a_async[3] = alu_mul(b[3], to_alu_t(im8), 0, 1, 0); break;
				case OP_MUL_8SU   : a_async[3] = alu_mul(b[3], to_alu_t(im8), 1, 0, 0); break;
				case OP_MUL_8S    : a_async[3] = alu_mul(b[3], to_alu_t(im8), 1, 1, 0); break;
				case OP_SHL_8U    : a_async[3] = alu_shl(b[3], to_alu_t(im8), 0); break;
				case OP_SHL_8S    : a_async[3] = alu_shl(b[3], to_alu_t(im8), 1); break;
				case OP_ROL_8     : a_async[3] = alu_rol(b[3], to_alu_t(im8)); break;
				case OP_POW_8     : a_async[3] = alu_pow(to_alu_t(im8)); break;
				//
				case OP_ADD_16U   : a_async[3] = alu_add(b[3], to_alu_t(im16), 0, 0, 0); break;
				case OP_ADD_16US  : a_async[3] = alu_add(b[3], to_alu_t(im16), 0, 1, 0); break;
				case OP_ADD_16SU  : a_async[3] = alu_add(b[3], to_alu_t(im16), 1, 0, 0); break;
				case OP_ADD_16S   : a_async[3] = alu_add(b[3], to_alu_t(im16), 1, 1, 0); break;
				case OP_SUB_16U   : a_async[3] = alu_sub(b[3], to_alu_t(im16), 0, 0, 0); break;
				case OP_SUB_16US  : a_async[3] = alu_sub(b[3], to_alu_t(im16), 0, 1, 0); break;
				case OP_SUB_16SU  : a_async[3] = alu_sub(b[3], to_alu_t(im16), 1, 0, 0); break;
				case OP_SUB_16S   : a_async[3] = alu_sub(b[3], to_alu_t(im16), 1, 1, 0); break;
				case OP_SBR_16U   : a_async[3] = alu_sbr(b[3], to_alu_t(im16), 0, 0, 0); break;
				case OP_SBR_16US  : a_async[3] = alu_sbr(b[3], to_alu_t(im16), 0, 1, 0); break;
				case OP_SBR_16SU  : a_async[3] = alu_sbr(b[3], to_alu_t(im16), 1, 0, 0); break;
				case OP_SBR_16S   : a_async[3] = alu_sbr(b[3], to_alu_t(im16), 1, 1, 0); break;
				case OP_MUL_16U   : a_async[3] = alu_mul(b[3], to_alu_t(im16), 0, 0, 0); break;
				case OP_MUL_16US  : a_async[3] = alu_mul(b[3], to_alu_t(im16), 0, 1, 0); break;
				case OP_MUL_16SU  : a_async[3] = alu_mul(b[3], to_alu_t(im16), 1, 0, 0); break;
				case OP_MUL_16S   : a_async[3] = alu_mul(b[3], to_alu_t(im16), 1, 1, 0); break;
				//
				case OP_AND_16    : a_async[3] = alu_and(b[3], to_alu_t(im16)); break;
				case OP_ORR_16    : a_async[3] = alu_orr(b[3], to_alu_t(im16)); break;
				case OP_XOR_16    : a_async[3] = alu_xor(b[3], to_alu_t(im16)); break;
			}
		}
		/////////////
		// stage 4 //
		/////////////
		if (!clt[4] && !irq[4]) {  // not clt or irq
			int8_t im8 = oc[4] >> 16;
			int16_t im16 = oc[4] >> 16;
			uint8_t im8u = im8;
			uint16_t im16u = im16;
			switch (oc[4] & 0xFF) { 
				case OP_LIT       : a_async[4] = to_alu_t(main_mem.rd(pcr_4, 4)); break;
				case OP_LIT_HS    : a_async[4] = to_alu_t(int16_t(main_mem.rd(pcr_4, 2))); break;
				case OP_LIT_HU    : a_async[4] = to_alu_t(main_mem.rd(pcr_4, 2)); break;
				case OP_LIT_BS    : a_async[4] = to_alu_t(int8_t(main_mem.rd(pcr_4, 1))); break;
				case OP_LIT_BU    : a_async[4] = to_alu_t(main_mem.rd(pcr_4, 1)); break;
				//
				case OP_MEM_R     : a_async[4] = to_alu_t(main_mem.rd(b[4].val, 4)); break;
				case OP_MEM_RHS   : a_async[4] = to_alu_t(int16_t(main_mem.rd(b[4].val, 2))); break;
				case OP_MEM_RHU   : a_async[4] = to_alu_t(main_mem.rd(b[4].val, 2)); break;
				case OP_MEM_RBS   : a_async[4] = to_alu_t(int8_t(main_mem.rd(b[4].val, 1))); break;
				case OP_MEM_RBU   : a_async[4] = to_alu_t(main_mem.rd(b[4].val, 1)); break;
				//
				case OP_MEM_8R    : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im8u, 4)); break;
				case OP_MEM_8RHS  : a_async[4] = to_alu_t(int16_t(main_mem.rd(b[4].val + im8u, 2))); break;
				case OP_MEM_8RHU  : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im8u, 2)); break;
				case OP_MEM_8RBS  : a_async[4] = to_alu_t(int8_t(main_mem.rd(b[4].val + im8u, 1))); break;
				case OP_MEM_8RBU  : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im8u, 1)); break;
				//
				case OP_MEM_16R   : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im16u, 4)); break;
				case OP_MEM_16RHS : a_async[4] = to_alu_t(int16_t(main_mem.rd(b[4].val + im16u, 2))); break;
				case OP_MEM_16RHU : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im16u, 2)); break;
				case OP_MEM_16RBS : a_async[4] = to_alu_t(int8_t(main_mem.rd(b[4].val + im16u, 1))); break;
				case OP_MEM_16RBU : a_async[4] = to_alu_t(main_mem.rd(b[4].val + im16u, 1)); break;
				//
				case OP_MEM_I8R   : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im8, 4)); break;
				case OP_MEM_I8RHS : a_async[4] = to_alu_t(int16_t(main_mem.rd(pcr_4 + im8, 2))); break;
				case OP_MEM_I8RHU : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im8, 2)); break;
				case OP_MEM_I8RBS : a_async[4] = to_alu_t(int8_t(main_mem.rd(pcr_4 + im8, 1))); break;
				case OP_MEM_I8RBU : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im8, 1)); break;
				//
				case OP_MEM_I16R   : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im16, 4)); break;
				case OP_MEM_I16RHS : a_async[4] = to_alu_t(int16_t(main_mem.rd(pcr_4 + im16, 2))); break;
				case OP_MEM_I16RHU : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im16, 2)); break;
				case OP_MEM_I16RBS : a_async[4] = to_alu_t(int8_t(main_mem.rd(pcr_4 + im16, 1))); break;
				case OP_MEM_I16RBU : a_async[4] = to_alu_t(main_mem.rd(pcr_4 + im16, 1)); break;
			}
		}
		/////////////
		// stage 5 //
		/////////////
		psh_async_5 = op_psh(clt[5], irq[5], oc[5]);  // push
	}

	//////////
	// sync //
	//////////
	else {
		// default data copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			a[(thd + 1) & THD_MSK] = a_async[thd];
			b[(thd + 1) & THD_MSK] = b_async[thd];
			//
			pop_er[(thd + 1) & THD_MSK] = pop_er_async[thd];
			psh_er[(thd + 1) & THD_MSK] = psh_er_async[thd];
		}
		////////////////
		// stage 0=>1 //
		////////////////
		// do cls
		for (int32_t stk = 0; stk < STACKS; stk++) {
			if(cls_async_0[stk]) { stacks[id_async_0].cls(stk); }
		}
		////////////////
		// stage 1=>2 //
		////////////////
		// do pop
		bool pop_er_2 = false;  // default is no error
		for (int32_t stk = 0; stk < STACKS; stk++) {
			if(pop_async_1[stk]) { pop_er_2 |= stacks[id_async_1].pop(stk); }
		}
		// inject into ring
		pop_er[2] = pop_er_2;
		////////////////
		// stage 3=>4 //
		////////////////
		if (!clt_async_3 && !irq_async_3) {  // not clt or irq
			int8_t im8 = oc_async_3 >> 16;
			int16_t im16 = oc_async_3 >> 16;
			uint8_t im8u = im8;
			uint16_t im16u = im16;
			switch (oc_async_3 & 0xFF) { 
				case OP_MEM_W     : main_mem.wr(b_async[3].val, 4, a_async[3].val); break;
				case OP_MEM_WH    : main_mem.wr(b_async[3].val, 2, a_async[3].val); break;
				case OP_MEM_WB    : main_mem.wr(b_async[3].val, 1, a_async[3].val); break;
				//
				case OP_MEM_8W    : main_mem.wr(b_async[3].val + im8u, 4, a_async[3].val); break;
				case OP_MEM_8WH   : main_mem.wr(b_async[3].val + im8u, 2, a_async[3].val); break;
				case OP_MEM_8WB   : main_mem.wr(b_async[3].val + im8u, 1, a_async[3].val); break;
				//
				case OP_MEM_16W   : main_mem.wr(b_async[3].val + im16u, 4, a_async[3].val); break;
				case OP_MEM_16WH  : main_mem.wr(b_async[3].val + im16u, 2, a_async[3].val); break;
				case OP_MEM_16WB  : main_mem.wr(b_async[3].val + im16u, 1, a_async[3].val); break;
				//
				case OP_MEM_I8W   : main_mem.wr(pcr_async_3 + im8, 4, a_async[3].val); break;
				case OP_MEM_I8WH  : main_mem.wr(pcr_async_3 + im8, 2, a_async[3].val); break;
				case OP_MEM_I8WB  : main_mem.wr(pcr_async_3 + im8, 1, a_async[3].val); break;
				//
				case OP_MEM_I16W  : main_mem.wr(pcr_async_3 + im16, 4, a_async[3].val); break;
				case OP_MEM_I16WH : main_mem.wr(pcr_async_3 + im16, 2, a_async[3].val); break;
				case OP_MEM_I16WB : main_mem.wr(pcr_async_3 + im16, 1, a_async[3].val); break;
			}
		}
		////////////////
		// stage 5=>6 //
		////////////////
		// do push
		bool psh_er_6 = false;  // default is no error
		for (int32_t stk = 0; stk < STACKS; stk++) {
			if(psh_async_5[stk]) { psh_er_6 |= stacks[id_async_5].psh(stk, a_async[5]); }
		}
		// inject into ring
		psh_er[6] = psh_er_6;
		////////////////
		// stage 7=>0 //
		////////////////
		stacks[id_async_7].hist_upd();

	}

}

