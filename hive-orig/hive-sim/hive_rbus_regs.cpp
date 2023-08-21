// hive_rbus_regs.cpp
#pragma once

// return opcode error status
bool op_error(bool clt_i, bool irq_i, uint32_t oc_i) {
	bool er_f = true;  // default is error
	if (!clt_i && !irq_i) {  // not clt or irq
		switch (oc_i & 0xFF) { 
			case OP_NOP       :
			case OP_HLT       :
			//
			case OP_POP       : 
			case OP_CLS       : 
			//
			case OP_JMP_8     :
			case OP_JMP_16    :
			case OP_JMP_24    :
			//
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
			case OP_JMP_Z     :
			case OP_JMP_NZ    :
			case OP_JMP_LZ    :
			case OP_JMP_NLZ   :
			case OP_JMP       :
			case OP_GTO       :
			case OP_IRT       :
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
			case OP_SHL_S     :
			case OP_SHL_U     :
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
			case OP_MEM_W     :
			case OP_MEM_WH    :
			case OP_MEM_WB    :
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
			case OP_REG_W     :
			//
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
			case OP_XOR_8     :
			//
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
			case OP_XOR_16     : er_f = false; break; 
		}
	}
	return (er_f);
}


// regs read, don't change stuff if not an active read
int32_t rbus_regs_rd(uint8_t addr_i, bool active_i) { 
	uint8_t rd_addr = addr_i & RBUS_MSK; 
	int32_t rd_data = 0;
	switch (rd_addr) {
		case RBUS_VECT:
			rd_data = armed.to_ulong();
			rd_data |= insvc.to_ulong() << THREADS;
			rd_data |= VER << (THREADS * 2);
		break;
		case RBUS_TIME :
			rd_data = time_2;
		break;
		case RBUS_ERROR:
			rd_data = error_reg;
		break;
		case RBUS_IO:
			rd_data = io_rd_reg;
		break;
		case RBUS_UART_TX:
			rd_data = (uart_tx_fifo.full() << 31);
			if (!active_i) { rd_data |= (uart_tx_reg & 0xFF); }
		break;
		case RBUS_UART_RX:
			rd_data = (uart_rx_fifo.empty() << 31) | uart_rx_fifo.top();
			if (active_i) { uart_rx_fifo.pop(); }
		break;
		case RBUS_SPI:
			rd_data = spi_rd_reg;
		break;
		case RBUS_SPDIF_L:
			rd_data = spdif_l_rd_reg;
		break;
		case RBUS_SPDIF_R:
			rd_data = spdif_r_rd_reg;
		break;
		case RBUS_PITCH:
			rd_data = pitch_rd_reg;
		break;
		case RBUS_VOLUME:
			rd_data = volume_rd_reg;
		break;
		case RBUS_LCD:
			rd_data = (lcd_fifo.full() << 31);
			if (!active_i) { rd_data |= (lcd_wr_reg & 0x1FF); }
		break;
		case RBUS_ENC:
			rd_data = enc_rd_reg;
		break;
	}
	return (rd_data);
}


// regs read write data
int32_t rbus_regs_io(uint8_t addr_i) { 
	uint8_t rd_addr = addr_i & RBUS_MSK; 
	int32_t rd_data = 0;
	switch (rd_addr) {
		case RBUS_IO:
			rd_data = io_wr_reg;
		break;
		case RBUS_UART_RX:
			rd_data = uart_rx_reg;
		break;
		case RBUS_UART_TX:
			rd_data = uart_tx_fifo.top();
		break;
		case RBUS_SPI:
			rd_data = spi_wr_reg;
		break;
		case RBUS_TUNER:
			rd_data = tuner_wr_reg;
		break;
		case RBUS_LCD:
			rd_data = lcd_fifo.top();
		break;
	}
	return (rd_data);
}


// clock
void rbus_regs_clk(bool clk_phase_i) {

	//////////////////
	// static state //
	//////////////////
	static bitset<THREADS> armed_async;
	static bitset<THREADS> insvc_async;
	static bitset<THREADS> clt_pend_async;
	static bitset<THREADS> irq_pend_async;
	static bitset<THREADS> clt_sr_async;
	static bitset<THREADS> irq_sr_async;
	static bitset<THREADS> clt_async;
	static bitset<THREADS> irq_async;
	static int32_t rbus_rd_data_async_2;
	static uint32_t oc_async_1;
	static alu_t a_async_1, b_async_1;
	static uint32_t errors_async_1;
	static uint8_t id_async_7;

	///////////
	// async //
	///////////
	if (!clk_phase_i) {
		// default data copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			armed_async[thd] = armed[thd];
			insvc_async[thd] = insvc[thd];
			//
			clt_pend_async[thd] = clt_pend[thd];
			irq_pend_async[thd] = irq_pend[thd];
			//
			clt_sr_async[thd] = clt_sr[thd];
			irq_sr_async[thd] = irq_sr[thd];
			//
			clt_async[thd] = clt[thd];
			irq_async[thd] = irq[thd];
		}
		oc_async_1 = oc[1];
		a_async_1 = a[1];
		b_async_1 = b[1];
		id_async_7 = id[7];
		/////////////
		// stage 1 //
		/////////////
		bitset<THREADS> irt_1 = 0;
		bool wr_f = false;
		uint8_t wr_addr = 0;
		if (!clt[1] && !irq[1]) {  // not clt or irq
			switch (oc[1] & 0xFF) { 
				case OP_REG_W     : 
					wr_f = true; 
					wr_addr = b[1].val & RBUS_MSK;
				break;
				case OP_REG_8W    : 
					wr_f = true; 
					wr_addr = (oc[1] >> 16) & RBUS_MSK;
				break;
				case OP_IRT       : 
					irt_1[id[1]] = true;
				break;
			}
		}
		//
		// VECTOR PROCSSING
		bool wr_vect_f = (wr_f && (wr_addr == RBUS_VECT));
		// one-hot enables
		bitset<THREADS> en_1 = 1 << id[1];
		bitset<THREADS> en_2 = 1 << id[2];
		// decode fields
		bitset<THREADS> wr_arm = a[1].val;
		bitset<THREADS> wr_dis = a[1].val >> THREADS;
		bitset<THREADS> wr_isr = a[1].val >> THREADS * 2;
		bitset<THREADS> wr_clt = a[1].val >> THREADS * 3;
		// loop thru bits:
		for ( int32_t thd = 0; thd < THREADS; thd++ ) {
			/////////////////////////
			// do 1st (precedence) //
			/////////////////////////
			// set arm @ write
			if ( wr_vect_f & wr_arm[thd] ) 
				armed_async[thd] = 1;
			// set service state @ irq
			if ( ( wr_vect_f & wr_isr[thd] ) | ( xsr_i[thd] & armed[thd] ) | isr_i[thd] ) 
				insvc_async[thd] = 1;
			// clear pending @ issue
			if ( en_2[thd] ) {
				irq_pend_async[thd] = 0;
				clt_pend_async[thd] = 0;
			}
			/////////////////////////
			// do 2nd (precedence) //
			/////////////////////////
			// clear arm @ write or clear
			if ( ( wr_vect_f & wr_dis[thd] ) | ( en_1[thd] & clt[1] ) ) 
				armed_async[thd] = 0;
			// clear service state @ clear or return
			if ( en_1[thd] & ( clt[1] | irt_1[thd] ) ) 
				insvc_async[thd] = 0;
			// set pending irq @ irq
			if ( ~insvc[thd] & ( ( wr_vect_f & wr_isr[thd] ) | ( xsr_i[thd] & armed[thd] ) | isr_i[thd] ) ) 
				irq_pend_async[thd] = 1;
			// set pending clt @ core clear or write
			if ( cla_i | ( wr_vect_f & wr_clt[thd] ) ) 
				clt_pend_async[thd] = 1;
		}  // bit loop end
		//
		// ERROR PROCESSING
		errors_async_1 = uint32_t(pop_er[1]) << id[1];
		errors_async_1 |= uint32_t(psh_er[1]) << (id[1] + THREADS);
		bool op_er_1 = op_error(clt[1], irq[1], oc[1]);
		errors_async_1 |= uint32_t(op_er_1) << (id[1] + (THREADS * 2));
		/////////////
		// stage 2 //
		/////////////
		bool rd_f = false;
		uint8_t rd_addr = 0;
		switch (oc[2] & 0xFF) { 
			case OP_REG_R     : 
				rd_f = true;
				rd_addr = b[2].val; 
			break;
			case OP_REG_8R    : 
				rd_f = true;
				rd_addr = oc[2] >> 16;	
			break;
		}
		if (rd_f) {  // active read
			rbus_rd_data_async_2 = rbus_regs_rd(rd_addr, true);
		}
		// decode & delay outputs
		clt_sr_async[2] = clt_pend[id[2]];
		irq_sr_async[2] = irq_pend[id[2]];
		/////////////
		// stage 7 //
		/////////////
		// inject into ring
		clt_async[7] = clt_sr[7];
		irq_async[7] = irq_sr[7];
	}

	//////////
	// sync //
	//////////
	else {  
		// default data copy
		for (int32_t thd = 0; thd < THREADS; thd++) {
			cla_i = false;  // reset after use
			xsr_i = 0;
			isr_i = 0;
			//
			armed[thd] = armed_async[thd];
			insvc[thd] = insvc_async[thd];
			irq_pend[thd] = irq_pend_async[thd];
			clt_pend[thd] = clt_pend_async[thd];
			//
			clt_sr[(thd + 1) & THD_MSK] = clt_sr_async[thd];
			irq_sr[(thd + 1) & THD_MSK] = irq_sr_async[thd];
			//
			clt[(thd + 1) & THD_MSK] = clt_async[thd];
			irq[(thd + 1) & THD_MSK] = irq_async[thd];
		}
		rbus_rd_data_3 = rbus_rd_data_async_2;
		clt_0[id_async_7] = clt_async[7];
		irq_0[id_async_7] = irq_async[7];
		////////////////
		// stage 1=>2 //
		////////////////
		if (!clt_async[1] && !irq_async[1]) {  // not clt or irq
			bool wr_f = false;
			uint8_t wr_addr;
			switch (oc_async_1 & 0xFF) { 
				case OP_REG_W     : 
					wr_f = true; 
					wr_addr = b_async_1.val & RBUS_MSK;
				break;
				case OP_REG_8W    : 
					wr_f = true; 
					wr_addr = (oc_async_1 >> 16) & RBUS_MSK;
				break;
			}
			if (wr_f) {
				switch (wr_addr) {
					case RBUS_ERROR:
						error_reg &= ~a_async_1.val;  // cow1
					break;
					case RBUS_IO:
						io_wr_reg = a_async_1.val;
					break;
					case RBUS_UART_TX:
						uart_tx_fifo.psh(a_async_1.val & 0xFF);
						uart_tx_reg = a_async_1.val & 0xFF;  // for display purposes
					break;
					case RBUS_TUNER:
						tuner_wr_reg = a_async_1.val;
					break;
					case RBUS_LCD:
						lcd_fifo.psh(a_async_1.val);
						lcd_wr_reg = a_async_1.val;  // for display purposes
					break;
				}
			}
			// update errors after write (precedence)
			error_reg |= errors_async_1;
		}
	}
}

// draw the registers
void rbus_regs_draw(short x_i, short y_i, string radix_i) { 

	// constants
	const int32_t REGS = 15;

	// draw header
	con_xy(x_i, y_i); 
	con_font('i');  // inverse
	cout << setw(5) << right << "LVL";
	cout << setw(12) << right << "I/O";
	cout << setw(12) << right << "RD";
	cout << setw(8) << right << "REG";
	cout << setw(3) << right << "A";

	// draw listing
	con_font('n');
	for (int32_t reg = 0; reg < REGS; reg++) {
		con_xy(x_i, y_i + 1 + reg);
		// LVL
		switch (reg) {
			case RBUS_UART_TX :
				cout << setw(5) << right << dec << uart_tx_fifo.level();
			break;
			case RBUS_UART_RX :
				cout << setw(5) << right << dec << uart_rx_fifo.level();
			break;
			case RBUS_LCD :
				cout << setw(5) << right << dec << lcd_fifo.level();
			break;
			default :
				cout << setw(5) << right << ".";
			break;
		}
		// I/O
		switch (reg) {
			case RBUS_VECT:
			case RBUS_TIME :
			case RBUS_ERROR:
			case RBUS_SPDIF_L:
			case RBUS_SPDIF_R:
			case RBUS_PITCH:
			case RBUS_VOLUME:
			case RBUS_ENC:
				cout << setw(12) << right << ".";
			break;
			default :
				cout << setw(12) << right << int_to_str(rbus_regs_io(reg), radix_i, 32, true);
			break;
		}
		cout << setw(12) << right << int_to_str(rbus_regs_rd(reg, false), radix_i, 32, true);
		cout << setw(8) << right << REG_NAMES[reg];
		cout << setw(3) << right << reg;
	}
}




