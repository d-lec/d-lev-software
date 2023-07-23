// hive_ctrl_ring.cpp
#pragma once

// clock
void ctrl_ring_clk(bool clk_phase_i) {
	
	//////////////////
	// static state //
	//////////////////
	static uint8_t id_async[THREADS];
	static uint32_t time_async_1;
	static uint32_t oc_async[THREADS];
	static uint32_t pc_async[THREADS];
	static uint32_t pcr_async_2, pcr_async_3;
	
	///////////
	// async //
	///////////
	if (!clk_phase_i) {
		// default copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			id_async[thd] = id[thd];
			oc_async[thd] = oc[thd];
			pc_async[thd] = pc[thd];
		}
		id_async[1] = (time_2 + 1) & THD_MSK;
		time_async_1 = time_2 + 1;
		oc_async[5] = main_mem.rd(pc[5], 4);  // fetch 4 bytes
		//
		pcr_async_2 = pc[2];  // PCR thru
		pcr_async_3 = pcr_3;
		/////////////
		// stage 0 //
		/////////////
		pc_async[0] = pc[0] + 2;  // +2 default
		if (clt[0]) {  // thread clear
			pc_async[0] = CLT_BASE | (id[0] << CLT_SPAN);
		}
		else if (irq[0]) {  // no change for irq
			pc_async[0] = pc[0];
		}
		else {
			switch (oc[0] & 0xFF) { 
				case OP_NOP       : pc_async[0] = pc[0] + 1; break;
				case OP_HLT       : pc_async[0] = pc[0]; break;
				//
				case OP_JMP_16    :
				case OP_JMP_8Z    :
				case OP_JMP_8NZ   :
				case OP_JMP_8LZ   :
				case OP_JMP_8NLZ  :
				case OP_JMP_8O    :
				case OP_JMP_8NO   :
				case OP_JMP_8E    :
				case OP_JMP_8NE   :
				case OP_JMP_8LS   :
				case OP_JMP_8NLS  :
				case OP_JMP_8LU   :
				case OP_JMP_8NLU  :
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
				case OP_SHL_8S    : 
				case OP_SHL_8U    :
				case OP_ROL_8     : 
				case OP_POW_8     : 
				//
				case OP_MEM_8R    :
				case OP_MEM_8W    :
				case OP_MEM_8WH   :
				case OP_MEM_8WB   :
				case OP_MEM_8RHS  :
				case OP_MEM_8RHU  :
				case OP_MEM_8RBS  :
				case OP_MEM_8RBU  :
				//
				case OP_MEM_I8R   :
				case OP_MEM_I8W   :
				case OP_MEM_I8WH  :
				case OP_MEM_I8WB  :
				case OP_MEM_I8RHS :
				case OP_MEM_I8RHU :
				case OP_MEM_I8RBS :
				case OP_MEM_I8RBU :
				//
				case OP_REG_8R    :
				case OP_REG_8W    :
				//
				case OP_AND_8     :
				case OP_ORR_8     :
				case OP_XOR_8     : pc_async[0] = pc[0] + 3; break;
				//
				case OP_JMP_24    :
				case OP_JMP_16Z   :
				case OP_JMP_16NZ  :
				case OP_JMP_16LZ  :
				case OP_JMP_16NLZ :
				case OP_JMP_16O   :
				case OP_JMP_16NO  :
				case OP_JMP_16E   :
				case OP_JMP_16NE  :
				case OP_JMP_16LS  :
				case OP_JMP_16NLS :
				case OP_JMP_16LU  :
				case OP_JMP_16NLU :
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
				case OP_MEM_16W    :
				case OP_MEM_16WH   :
				case OP_MEM_16WB   :
				case OP_MEM_16RHS  :
				case OP_MEM_16RHU  :
				case OP_MEM_16RBS  :
				case OP_MEM_16RBU  :
				//
				case OP_MEM_I16R   :
				case OP_MEM_I16W   :
				case OP_MEM_I16WH  :
				case OP_MEM_I16WB  :
				case OP_MEM_I16RHS :
				case OP_MEM_I16RHU :
				case OP_MEM_I16RBS :
				case OP_MEM_I16RBU :
				//
				case OP_AND_16     :
				case OP_ORR_16     :
				case OP_XOR_16     : pc_async[0] = pc[0] + 4; break;
			}
		}
		/////////////
		// stage 2 //
		/////////////
		if (!clt[2] && irq[2]) { 
			pc_async[2] = IRQ_BASE | (id[2] << IRQ_SPAN);
		}
		/////////////
		// stage 3 //
		/////////////
		if (!clt[3] && !irq[3]) {  // no clt or irq
			switch (oc[3] & 0xFF) { 
				case OP_JMP_8     : pc_async[3] = pc[3] + bmods(oc[3] >> 8, 8); break;
				case OP_JMP_16    : pc_async[3] = pc[3] + bmods(oc[3] >> 8, 16); break;
				case OP_JMP_24    : pc_async[3] = pc[3] + bmods(oc[3] >> 8, 24); break;
				case OP_LIT       : pc_async[3] = pc[3] + 4; break;
				case OP_LIT_HS    : pc_async[3] = pc[3] + 2; break;
				case OP_LIT_HU    : pc_async[3] = pc[3] + 2; break;
				case OP_LIT_BS    : pc_async[3] = pc[3] + 1; break;
				case OP_LIT_BU    : pc_async[3] = pc[3] + 1; break;
				//
				case OP_JMP_Z     : if (a[3].val == 0) { pc_async[3] = pc[3] + b[3].val; } break;
				case OP_JMP_NZ    : if (a[3].val != 0) { pc_async[3] = pc[3] + b[3].val; } break;
				case OP_JMP_LZ    : if (a[3].val < 0) { pc_async[3] = pc[3] + b[3].val; } break;
				case OP_JMP_NLZ   : if (a[3].val >= 0) { pc_async[3] = pc[3] + b[3].val; } break;
				case OP_JMP       :
				case OP_JSB       : pc_async[3] = pc[3] + b[3].val; break; 
				case OP_GTO       : 
				case OP_IRT       : 
				case OP_GSB       : pc_async[3] = b[3].val; break;
				//
				case OP_JMP_8Z    : if (a[3].val == 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NZ   : if (a[3].val != 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8LZ   : if (a[3].val < 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NLZ  : if (a[3].val >= 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8O    : if (a[3].val & 1) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NO   : if (~a[3].val & 1) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8E    : if (a[3].val == b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NE   : if (a[3].val != b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8LS   : if (a[3].val < b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NLS  : if (a[3].val >= b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8LU   : if (uint32_t(a[3].val) < uint32_t(b[3].val)) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JMP_8NLU  : if (uint32_t(a[3].val) >= uint32_t(b[3].val)) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); } break;
				case OP_JSB_8     : pc_async[3] = pc[3] + bmods(oc[3] >> 16, 8); break;
				//
				case OP_JMP_16Z   : if (a[3].val == 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NZ  : if (a[3].val != 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16LZ  : if (a[3].val < 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NLZ : if (a[3].val >= 0) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16O   : if (a[3].val & 1) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NO  : if (~a[3].val & 1) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16E   : if (a[3].val == b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NE  : if (a[3].val != b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16LS  : if (a[3].val < b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NLS : if (a[3].val >= b[3].val) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16LU  : if (uint32_t(a[3].val) < uint32_t(b[3].val)) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JMP_16NLU : if (uint32_t(a[3].val) >= uint32_t(b[3].val)) { pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); } break;
				case OP_JSB_16    : pc_async[3] = pc[3] + bmods(oc[3] >> 16, 16); break;
			}
		}
	}

	//////////
	// sync //
	//////////
	else {
		// default copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			id[(thd + 1) & THD_MSK] = id_async[thd];
			oc[(thd + 1) & THD_MSK] = oc_async[thd];
			pc[(thd + 1) & THD_MSK] = (pc_async[thd]) & PC_MSK;
		}
		oc_0[id_async[7]] = oc_async[7];
		pc_0[id_async[7]] = pc_async[7];
		time_2 = time_async_1;
		pcr_3 = pcr_async_2;
		pcr_4 = pcr_async_3;
	}
}

