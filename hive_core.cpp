// hive_core.cpp
#pragma once

class core {

public:
	
	// state holders
	uint8_t id[THREADS];
	uint32_t time_2;
	uint32_t oc[THREADS];
	uint32_t pc[THREADS];
	uint32_t pcr_3, pcr_4;
	alu_t a[THREADS];
	alu_t b[THREADS];
	int32_t rbus_rd_data_3;
	bitset<THREADS> pop_er;
	bitset<THREADS> psh_er;
	bool cla_i;
	bitset<THREADS> xsr_i;
	bitset<THREADS> isr_i;
	bitset<THREADS> clt;
	bitset<THREADS> irq;
	bitset<THREADS> clt_pend;
	bitset<THREADS> irq_pend;
	bitset<THREADS> clt_sr;
	bitset<THREADS> irq_sr;
	bitset<THREADS> armed;
	bitset<THREADS> insvc;
	uint32_t error_reg;
	uint32_t io_wr_reg;
	uint32_t io_rd_reg;
	uint32_t uart_tx_reg;
	uint32_t uart_rx_reg;
	uint32_t spi_wr_reg;
	uint32_t spi_rd_reg;
	uint32_t spdif_l_rd_reg;
	uint32_t spdif_r_rd_reg;
	uint32_t pitch_rd_reg;
	uint32_t volume_rd_reg;
	uint32_t tuner_wr_reg;
	uint32_t lcd_wr_reg;
	uint32_t enc_rd_reg;
	// displayed state
	bool clt_0[THREADS];
	bool irq_0[THREADS];
	uint32_t oc_0[THREADS];
	uint32_t pc_0[THREADS];
	

	// register set fifos
	#include "hive_fifo.cpp"
	fifo uart_tx_fifo, uart_rx_fifo, lcd_fifo;

	// main mem
	#include "hive_main_mem.cpp"
	main_mem main_mem;

	// data stacks
	#include "hive_stacks.cpp"
	stacks stacks[THREADS];

	// physically included code
	#include "hive_ctrl_ring.cpp"
	#include "hive_alu_funcs.cpp"
	#include "hive_data_ring.cpp"
	#include "hive_rbus_regs.cpp"

	
	void rst() {
		armed = 0;
		insvc = 0;
		clt_pend = -1;  // note: assert @ async reset!
		irq_pend = 0;
		clt_sr = 0;
		irq_sr = 0;
		clt = 0;
		irq = 0;
		cla_i = false;
		xsr_i = 0;
		isr_i = 0;
		pop_er = 0;
		psh_er = 0;
		error_reg = 0;
		uart_tx_reg = 0;
		uart_rx_reg = 0;
		io_wr_reg = 0;
		io_rd_reg = 0;
		spi_wr_reg = 0;
		spi_rd_reg = 0;
		spdif_l_rd_reg = 0;
		spdif_r_rd_reg = 0;
		pitch_rd_reg = 0;
		volume_rd_reg = 0;
		tuner_wr_reg = 0;
		lcd_wr_reg = 0;
		enc_rd_reg = 0;
		uart_rx_fifo.rst();
		uart_tx_fifo.rst();
		lcd_fifo.rst();
		pcr_3 = CLT_BASE | (ID_RST[3] << CLT_SPAN);
		pcr_4 = CLT_BASE | (ID_RST[4] << CLT_SPAN);
		time_2 = ID_RST[2];
		for (int32_t thd = 0; thd < THREADS; thd++) { 
			id[thd] = ID_RST[thd];
			oc[thd] = 0;  // nop
			pc[thd] = CLT_BASE | (ID_RST[thd] << CLT_SPAN);
			oc_0[thd] = 0;  // nop
			pc_0[thd] = CLT_BASE | (thd << CLT_SPAN);
			a[thd].val = 0;
			b[thd].val = 0;
			stacks[thd].rst(); 
			clt_0[thd] = false;
			irq_0[thd] = false;
		}
	}


	bool cfg(string hal_rd_i, bool quiet_i, bool clean_i) {
		rst();
		uart_rx_fifo.cfg();
		uart_tx_fifo.cfg();
		lcd_fifo.cfg();
		for (int32_t thd = 0; thd < THREADS; thd++) { stacks[thd].z(); }
		return(main_mem.hal_parse(hal_rd_i, quiet_i));  // load hal file, return status
	}


	void clk() {
		///////////
		// async //
		///////////
		ctrl_ring_clk(0);
		data_ring_clk(0);
		rbus_regs_clk(0);

		//////////
		// sync //
		//////////
		ctrl_ring_clk(1);
		data_ring_clk(1);
		rbus_regs_clk(1);
	}

	
	void cycle() {
		for (int32_t thd = 0; thd < THREADS; thd++) { clk(); }
	}


	void draw_stack_info(short x_i, short y_i, uint8_t thd_i, string radix_i) { 

		// extract state
		bitset<STACKS> cls = op_cls(clt_0[thd_i], irq_0[thd_i], oc_0[thd_i]);
		bitset<STACKS> pop = op_pop(clt_0[thd_i], irq_0[thd_i], oc_0[thd_i]);
		bitset<STACKS> psh = op_psh(clt_0[thd_i], irq_0[thd_i], oc_0[thd_i]);
		
		// stacks: draw header
		con_xy(x_i, y_i);
		con_font('i');
		cout << setw(6) << right << "ERROR";
		cout << setw(4) << right << "CLS";
		cout << setw(4) << right << "POP";
		cout << setw(4) << right << "PSH";
		cout << setw(4) << right << "LVL";
		cout << setw(12) << right << "TOP";
		cout << setw(3) << right << "S";
		// draw data
		con_font('n');
		for (int32_t stk = 0; stk < STACKS; stk++) { 
			con_xy(x_i, y_i + 1 + stk);
			uint32_t level = stacks[thd_i].level_hist[stk];
			bool pop_er_f = stacks[thd_i].pop_er[stk];
			bool psh_er_f = stacks[thd_i].psh_er[stk];
			bool both_er_f = pop_er_f && psh_er_f;
			if (pop_er_f || psh_er_f) { con_font('e'); }
			cout << setw(6) << right << (both_er_f ? "!BTH!" : (pop_er_f ? "!POP!" : (psh_er_f ? "!PSH!" : ".")));
			con_font('n');
			cout << setw(4) << right << (cls[stk] ? "CLS" : ".");
			cout << setw(4) << right << (pop[stk] ? "POP" : ".");
			cout << setw(4) << right << (psh[stk] ? "PSH" : ".");
			cout << setw(4) << right << dec << (level ? int_to_str(level, "INT", 8, false) : ".");
			cout << setw(12) << right << (level ? int_to_str(stacks[thd_i].top_hist[stk].val, radix_i, 32, true) : ".");
			cout << setw(3) << right << stk;
		}
	}
	

};
