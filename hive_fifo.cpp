// hive_fifo.cpp
#pragma once

class fifo
{
private:
	
	// state and such
	uint32_t mem[FIFO_DEPTH];
	uint32_t wr_ptr; 
	uint32_t rd_ptr; 

	// zero out memory
	void zm() { 
		for(uint32_t addr = 0; addr < FIFO_DEPTH; addr++) {
			mem[addr] = 0; 
		}
	}

	// write memory
	void wr(uint32_t addr_i, uint32_t data_i) { 
		mem[addr_i & FIFO_ADDR_MSK] = data_i; 
	}

public:

	// reset
	void rst() { 
		wr_ptr = 0; 
		rd_ptr = 0; 
	}

	// config
	void cfg() { 
		rst(); 
		zm(); 
	}

	// read memory
	uint32_t rd(uint32_t addr_i) { 
		return(mem[addr_i & FIFO_ADDR_MSK]); 
	}

	// levels
	uint32_t level() { return(wr_ptr - rd_ptr); }
	bool empty() { return( level() == 0 ); };
	bool full() { return( level() >= FIFO_DEPTH ); };

	// read top
	uint32_t top() { return( rd(rd_ptr) ); }

	// read & pop
	int32_t pop() { 
		int32_t rd_data = top();
		if (!empty()) { rd_ptr++; }
		return(rd_data); 
	}

	// write & push
	void psh(int32_t data_i) { 
		if (!full()) { wr(wr_ptr, data_i); wr_ptr++; };
	}

};

