// hive_stacks.cpp
#pragma once

class stacks {
	
public:
	
	alu_t mem[STACKS][STK_DEPTH];
	uint32_t level[STACKS];
	bitset<STACKS> psh_er;
	bitset<STACKS> pop_er;
	// history
	alu_t top_hist[STACKS];
	uint32_t level_hist[STACKS];

	// clear a stack
	void cls(int32_t stk_i) { 
		level[stk_i] = 0; 
		pop_er[stk_i] = 0; 
		psh_er[stk_i] = 0; 
		//
		top_hist[stk_i].val = 0; 
		top_hist[stk_i].flg = 0; 
		level_hist[stk_i] = 0; 
	}
	
	// reset (clear) all stacks
	void rst() { 
		for (int32_t stk = 0; stk < STACKS; stk++) { 
			cls(stk); 
		}
	}
	
	// zero memory
	void z() { 
		for(int32_t stk = 0; stk < STACKS; stk++) {
			for(uint32_t ptr = 0; ptr < STK_DEPTH; ptr++) {
				mem[stk][ptr].val = 0; 
				mem[stk][ptr].flg = 0; 
			}
		}
	}
	
	// read stack top
	alu_t top(int32_t stk_i) { 
		return( mem[stk_i][level[stk_i] & STK_ADDR_MSK] );
	}
	
	// pop stack, return error
	bool pop(int32_t stk_i) { 
		bool empty = (level[stk_i] == 0);
		bool dec = (STK_PROT_POP) ? !empty : true;
		if (dec) { level[stk_i]--; }
		if (empty) { pop_er[stk_i] = true; }
		return( empty );
	}
	
	// push stack, return error
	bool psh(int32_t stk_i, alu_t data_i) { 
		bool full = (level[stk_i] == STK_DEPTH);
		bool inc = (STK_PROT_PSH) ? !full : true;
		if (inc) { 
			level[stk_i]++;
			mem[stk_i][level[stk_i] & STK_ADDR_MSK] = data_i; 
		};
		if (full) { psh_er[stk_i] = true; }
		return( full );
	}

	// history update
	void hist_upd() { 
		for (int32_t stk = 0; stk < STACKS; stk++) { 
			top_hist[stk] = top(stk); 
			level_hist[stk] = level[stk]; 
		}
	}

};

