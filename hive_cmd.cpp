// hive_cmd.cpp
#pragma once

// edit command line, return true if key_i used.
// enter inserts space @ end and puts cursor there.
// tab & other white space ignored.
// ctrl l&r arrows postion cursor at next word or beginning / end.
bool cmd_edit(string& cmd_str_io, uint32_t& cursor_pos_io, uint64_t key_i) {
	bool rtn_val = true;  // default true
	bool done_f = false;  // used to search for spaces
	// control chars:
	switch (key_i) {
		case KBD_ESC :
			cmd_str_io.clear();
			cursor_pos_io = 0;
		break;
		case KBD_LT :
			if (cursor_pos_io) { cursor_pos_io--; }
		break;
		case KBD_RT :
			if (cursor_pos_io < cmd_str_io.size()) { cursor_pos_io++; }
		break;
		case KBD_HOME :
			cursor_pos_io = 0;
		break;
		case KBD_END :
			cursor_pos_io = cmd_str_io.size();
		break;
		case KBD_BKSP :
			if (cursor_pos_io) { cmd_str_io.erase(--cursor_pos_io, 1); }
		break;
		case KBD_DEL :
			if (cursor_pos_io < cmd_str_io.size()) { cmd_str_io.erase(cursor_pos_io, 1); }
		break;
		case KBD_ENT :
			cmd_str_io += " ";
			cursor_pos_io = cmd_str_io.size();
		break;
		case KBD_CTRL_RT :
			while (!done_f) {
				if (cursor_pos_io < cmd_str_io.size()) {  // not @ end
					cursor_pos_io++;  // inc
					if ((cmd_str_io[cursor_pos_io-1] == ' ') && (cmd_str_io[cursor_pos_io] != ' ')) { done_f = true; }
				}
				else { done_f = true; }  // @ end
			}
		break;
		case KBD_CTRL_LT :
			while (!done_f) {
				if (cursor_pos_io > 1) {  // not @ start
					cursor_pos_io--;  // dec
					if ((cmd_str_io[cursor_pos_io-1] == ' ') && (cmd_str_io[cursor_pos_io] != ' ')) { done_f = true; }
				}
				else {  // @ start
					cursor_pos_io = 0;
					done_f = true; 
				} 
			}
		break;
		default: 
			rtn_val = false;
		break;
	}
	// non-white or space char:
	if (!rtn_val) {
		if ((ch_is_nonwhite(key_i)) || (key_i == ' ')) { 
			cmd_str_io.insert(cursor_pos_io++, 1, char(key_i)); 
			rtn_val = true;
		}
	}
		
	return(rtn_val);
}


// command line history circular buffer
// inc write pointer for each write, zero out read pointer & input command string
// inc / dec read pointer for each read (limited to 0 and writes depth).
template <uint32_t CMD_DEPTH>  // bs just to set a constant
class cmd_mem {
private:

	// state and such
	string mem[CMD_DEPTH];
	uint32_t wr_ptr = 0;
	uint32_t rd_ptr = 0;
	uint32_t writes = 0;

	// level & modulo (safe) indices
	uint32_t level() { return(wr_ptr - rd_ptr); }
	uint32_t rd_idx() { return(rd_ptr % CMD_DEPTH); }
	uint32_t wr_idx() { return(wr_ptr % CMD_DEPTH); }

public:

	// comand string => mem, clear cmd
	void wr(string& cmd_str_io) { 
		cmd_str_io = str_trim(cmd_str_io);  // trim whitespace at ends
		mem[wr_idx()] = cmd_str_io;  // sto cmd
		wr_ptr++;  // inc wr ptr
		rd_ptr = wr_ptr;  // reset rd ptr
		if (writes < CMD_DEPTH-1) { writes++; }  // inc write count
		mem[wr_idx()] = "";  // clear next slot
		cmd_str_io = mem[rd_idx()];  // clear cmd_str
	}

	// mem => comand string
	bool rd(string& cmd_str_io, bool deeper_f) { 
		bool rd_f = false;
		if (deeper_f) {  // go deeper
			if (level() < writes) { rd_ptr--; rd_f = true; }
		}
		else {  // come back
			if (level() > 0) { rd_ptr++; rd_f = true; }
		}
		if (rd_f) { cmd_str_io = mem[rd_idx()]; }
		return(rd_f);
	}
};

