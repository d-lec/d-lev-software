/*
hive_sim.cpp
- Started 2015-02-14 (eek!) by Eric Wallin
- No explicit project management, just compile this file.
- This is a complete re-write of the previous simulator,
  which was last updated on 2017-05-24 for core v10.17

================
!FIXME! / TODO / NOTE
- todo: ALU shift and rotate ops return flags=0 (s/b result of mult).
- todo: Add new registers & access to them.
- note: DON'T USE ISSPACE()! - crashes w/ bad input (use ch_is_white)
- note: DON'T USE STOL()! - crashes w/ bad input (use strtol w/ str_is_hex() check)
================

2021-02-23
- New boot code location.
- New CLT & ISR locations.

2020-09-15
- Commented out *.ttl file generation.

2020-08-06
- Broken with Linux Mint v20 update!
- Issue was getch(), replaced with read, added cout flush before sleep.
- Other minor improvements to input code.

2020-01-30
- Can now peek and poke memory w/ commands rb, rh, rm, wb, wh, wm.
- Can now read, pop, push, and write (pop & push) stacks.
- Merged command result and error message systems.

2020-01-26
- Replaced hive_console*.cpp with files from D-Lib.
- Added hive_view_file.cpp from D-Lib for help file viewing.
- More detailed error reporting system.
- Big switch statement for input key code is now a big if statement.
- Removed isspace() from input key processing and str_trim().

2020-01-03
- Generates *.spi file for D-Lib librarian SW pump use.

2019-12-14
- Code adapted for both MSWIN and POSIX compilation.
- For POSIX target comment out MSWIN define at beginning of this file.
- Fixed error in MSWIN key codes, which are now encoded little endian.
- Found redundant SHFT MSWIN key codes, most SHFT/CTRL moved to ALT.
- cmd history now uses red text for error conditions.
- All int => int32_t; all uint and unsigned int => uint32_t;

2019-11-27
- ESC now clears cmd line.
- Blue motif.
- Help file is now "SIM_HELP.txt".

2019-11-13
- SHFT L & R arrows now cycle thru the threads (this was broken).
- Added "help" command to display the SIM_QUICK_START.txt file.

2019-10-29
- Full-featured command line editing (via function and class in hive_console.cpp).
  - L & R arrows, CTRL L & R arrows, BKSP, DEL act as usual.
  - UP & DN arrows recall cmd history.
- F1-F4 now select threads 1-4, CTRL F1-F4 select threads 5,6,7,0.
- CTRL PGUP / PGDN now move listing one page.
- CTRL UP / DN now move listing one line.
- CTRL HOME now returns listing hilight to execution point.

2019-10-26
- Fixed minor bug in cmd line for "ru" and "rl", now give result after <OK>.

2019-10-25
- Rename (in hive_pkg): FLG_W => ALU_F_W
- Thd 4 stacks NOT erroring w/ verify.hal, sim error display is sticky!

2019-10-24
- fifo() and stack() chg: *_PTR_MSK => *_ADDR_MSK
- Using input buffer instead of queue for intermediate key code byte storage!
- Better screen clear ANSI code: doesn't scroll.
- Now revert kbd mode @ quit.
- Post assembly message states minimum console dims.
- Added 'r' command to refresh screen.
- Removed con_cls() from general redraw.
- File name change: hive_top.cpp => hive_core.cpp

2019-10-23
- fifo() is back to the way it was.
- Using queue for intermediate key code storage!
- I believe it was isspace() causing the segment fault, replaced with ch_is_white().

2019-10-22
- Developed substitutes of getch() and kbhit() for POSIX.
- Added build switch for POSIX (Unix) in package (not finished).
- Key codes are now defines.
- fifo() is now templated and not restricted to powers of 2 depth.
- Added "home", "quit", and <#> t (thread view) commands.
- Moved F9-F12 commands to F5-F8, removed F1-F8.
- Various minor edits here and there.
=> Multi-byte control keys are causing segment faults!

2019-09-29
- Work to adapt to Ubuntu, hive_console.cpp totally different.
- Assembly seems to work, keys need remapping.

2019-09-18
- Added back thread reset @ end of *.ttl file (fixed boot code).

2019-07-12
- Fixed discrepancy between HW and SW boot loader reserved mem.
- New constant BOOT_ADDR in package.
- Removed thread reset @ end of *.ttl file.

2019-01-22
- CRC32 now output on command line after config.

2019-01-14
- mem_to_ttl() now renames output TTL file to <crc>.ttl

2018-12-07
- Changes to reflect v13.04, bugs fixed, passes verification.

2018-12-06
- Changes to reflect v13.04.  HAS BUGS!

2018-11-04
- Removed CRC memory parameter in ttl file generation.

2018-08-21
- Changes to reflect v13.02:
  - Shuffled ops a bit.

2018-08-20 
- Fixed mult flags.
- Shuffled ops a bit.
- More verification tests, passes all of them.

2018-08-18
- Changes to reflect v13.01:
  - Stacks now carry arithmetic flags for OP_LIM & OP_SAT.
  - OP_ADD_*, OP_SUB_*, OP_MUL_* reorg & changes.
  - OP_AND_*, OP_ORR_*, OP_XOR_*, OP_SUB_R* removal.

2018-01-22
- Even more dinkin' around with the ttl scrip generation code for new EEPROM.

2018-01-17
- More dinkin' around with the ttl scrip generation code.

2018-01-17
- ttl scripting edits to reflect new "ws" HCL command.

2018-01-15
- New function in core_main_mem kicks out ttl script for programming SPI.
- Added crc_32() sub to pkg.

2017-12-16
- Added isr console command (force irq).

2017-12-14
- Changes to reflect v12.03:
  - Added OP_MUL_SU, OP_MUL_8SU, OP_MUL_16SU back.

2017-11-21
- Changes to reflect v12.02:
  - Added OP_ADD_*, OP_ORR_*, OP_XOR_* (8 & 16 im variants).
  - Removed OP_BRA, OP_BRO.
  - A bit of opcode reshuffling.

2017-11-20
- HAL double quoted strings now supported in mem reserve/init.

2017-11-19
- Removed HAL force to lower case, now case sensitive.

2017-11-18
- Fixed ch4 text lower case

2017-11-15
- Fixed tx uart bug.
- FIXME!: saw pgm crash with EOB followed by EOL at start of line!

2017-11-13
- Changes to reflect v12.01:
  - Added OP_SWP (32 bit byte endian swap).
  - Removed OP_CPY_*, OP_GTO_*.
  - Slight opcode reshuffling.

2017-11-05
- Default input file is now "theremin.hal" for development

2017-11-01
- Reordered preprocessing to put `define after puff, removed ` and @ from puff.
- Fixed LCD fifo (8=>9 bits wide), fifo data is now uint32_t.

2017-10-30
- `include again examines state file names for recursion.
- Unordered map just tracks unique file count.

2017-10-28
- Moved ASCII preprocessing to its own separate subroutine.

2017-10-26
- Added ASCII escape character support.  Moved ASCII processing to hal_pre_2().

2017-10-22
- Bug fix: hal decode collision.

2017-10-20
- Bug fixes: 
  - Unordered map now tracking unique included file names (can't use state file name).
  - Error flag guards around label proc code to preserve error index.

2017-10-19
- Hilite focus "addr[]" in hive_sim() is now scalar "disp_addr".
- Hilite focus mechanism now allows cfg, exection, & thread switch while examining memory.
- Fixed many bugs associated with new opcodes.

2017-10-18
- Changes to reflect v11.11:
  - Added OP_MEM_I8*, OP_MEM_16*, OP_MEM_I16*.
  - OP_MEM_#* IM now always byte offset (not scaled to access width). 
  - Added OP_JSB, OP_JSB_8, OP_JSB_16 syntax:
    - sa := pc += sb
    - sa := pc := addr, pb
- Removed redundant / confusing / dangerous HAL syntax forms:
  - JMP syntax:
    - pc +/-= im;                 - use: pc := @label
    - (sa?0) ? pc +/-= im, pb;    - use: (sa?0) ? pc := @label, pb
    - (sa?odd) ? pc +/-= im, pb;  - use: (sa?odd) ? pc := @label, pb
    - (sa?sb) ? pc +/-= im;       - use: (sa?sb) ? pc := @label
  - GSB syntax: 
    - pc := sb, sa := pc;         - use: sa := pc := sb

2017-10-16
- Changes to reflect v11.11:
  - Moved OP_LIT* to OP_MEM group.
  - Moved OP_PCR to OP_JMP etc. group.
  - Moved OP_JMP_8, OP_JMP_16, OP_JMP_24 to their own group.
  - Two operand conditional test select enumeration changed to remove gap.

2017-10-14
- A separate listing highlight now follows scroll focus.

2017-10-13
- Retooled hal_pre_0(): duplicate `include files (including current file) now passed over.
  This sidesteps both recursion and duplication (poor man's package system).
- UI: displayed file name now follows line number focus.
- A bit of tidying-up in core_main_mem.

2017-10-11
- Fixed minor idx_o calc (pointing to next rather than this).

2017-10-10
- Lots of tidying-up in the HAL assembly subroutines.
- A single source vector container is now passed down the gauntlet.
- Empty lines no longer removed.
- Debug file write code now a subroutine.

2017-10-09
- Implement dead code removal in hal_lbl_2():
  - Recursively kills scopes anchored by orphaned LH labels.
  - Reports scope & label counts, dead line count & passes.
- Improved some error reporting.

2017-10-08
- Implemented scoping support via {} syntax in hal_lbl_1().

2017-10-07
- Reserved mem syntax changed from "@lbl[#]:={#}" to "mem[#]:=(#)".
- Swapped hal_pre_1() and hal_pre_2() functional positions.
- @clt_#, @irq_# hard encoding removed, `include aliases now in hive_defines.hal.
- Subroutines hal_lbl_0() and hal_lbl_1() merged, less port activity.

2017-10-06
- C #include syntax switched to SV `include syntax.
- Added `define and `undef directive support.
- Parsing change: @ must be a prefix (no space between it and label).

2017-10-05
- Fixed tellg / seekg issue in hal_pre_0() by using fstream::pos_type and making input file open in binary mode.
- Added package function str_trim() to shave off any whitespace @ both ends of EOL string.
- # Include syntax now requires the filename to be contained within double quotes.

2017-10-01
- Generalized # pre-processor directive mechanism installed.
  - # include <filename> pulls in file like in C.
  - Protection against file recursion.
  - Compressing unused lines and unknown directives at hal_pre_2() stage.
- Lots of retooling, all temp file usage replaced with vect of struct.
- Error reporting revamped.
- Verification file updated with latest opcodes and subroutines.

2017-09-28
- Working on include #<filename> support.

2017-09-22
- Changes to reflect v11.10:
  - Added OP_SUB_R, OP_SUB_8R, OP_SUB_16R.
  - Removed OP_*_*US.
 - Slight opcode reshuffling.
 - bra, bro, brx now back to returning 0,-1.

2017-09-13
- Changes to reflect v11.09:
  - Added OP_JMP_16, OP_JMP_24, w/o pop.
  - Removed pop option from OP_JMP_8 HAL patterns.
 
2017-09-13
- Removed all "clumping" in core_main_mem.hal_pre_2().

2017-09-12
- Changes to reflect v11.08:
  - Added OP_HLT (infinite loop).
  - Added IM jump conditionals: OP_JMP_#LU, OP_JMP_#NLU, OP_JMP_#O, OP_JMP_#NO.
  - IM jump ops renamed: _#LT => *_#LS; *_#NLT => *_#NLS.
  - HAL AB comparison syntax is now signed / unsigned where necessary (<s, <u, !<s, !<u).

2017-09-10
- Changes to reflect v11.07:
  - *_*SU is now *_*US.
  - Slight opcode reshuffling.
  - bra, bro, brx now return 0,1.

2017-09-10
- [F1:F7] => thread[1:7]; F8 => thread 0.
- General renaming (remove preference for signedness): 
  - LOW_W => HLF_W.
  - l => h.
  - xsu => su.
  - xu => u.
  - x => s.

2017-09-08
- Command line recall w/ CTRL-UP (of only the last successful command).
- All input whitespace from kbd now replaced with space.
- TODO:
  - Flesh out verify.hal to cover latest opcode changes.
  - Complete the register set manipulation commands.

2017-09-06
- HAL parse seems to be working, sim & SV verification pass.
- Edited str_is_int() and str_to_int() in package, now accept leading '+'.
- Stack inc and dec now show ++ and -- in listing.
- Restructured the preprocessing steps somewhat.
- TODO:
  - Flesh out verify.hal to cover latest opcode changes.
  - Complete the register set manipulation commands.
  - Maybe: command line recall (of only the last command).
  
2017-09-04 - BROKEN!  Need to work on HAL parse inflate/deflate.
- First build to reflect v11.06 (for improved opcode orthogonality).
- Removed "wf" command, MIF files write after successful assembly.
- Added fifo to LCD reg.
- Prettied up some listing output.
- All id_async[] now static local.

2017-09-02
- Added assembly results screen @ cfg.

2017-08-12
- Added Theremin registers, fixme.
- Thinking of {#r "rr"} and {# #r "wr"} generic register commands.

2017-08-11
- Clean cfg screen shows results of assembly.
- Added "wf" command to write FPGA MIF files.
- Simplified command line token parse logic.
- TODO:
  - README.TXT with commands and other info in it.

2017-08-10
- First build to reflect v11.05.
- Seems to work OK, caught some errors in the verification file.
*/


// comment this out for POSIX compile
// #define MSWIN

//replacements for contents of bits/stdc++.h
#include <thread>
#include <sstream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>

#include <stdint.h>
#ifdef MSWIN
  #include <conio.h>
  #include <windows.h>
#endif

using namespace std;

// hive stuff
#include "hive_pkg.cpp"
#ifdef MSWIN
  #include "hive_console_mswin.cpp"
#else
  #include <iomanip>
  #include "hive_console_linux.cpp"
#endif
#include "hive_cmd.cpp"
#include "hive_core.cpp"
#include "hive_view_file.cpp"


//////////
// MAIN //
//////////

int main(int argc, char* argv[]) {

	// help file
	string help_file = "sim_help.txt";

	// default input file
	string rd_file = "theremin.hal";
//	string rd_file = "verify.hal";

	// if there are command line arguments
	if (argc > 1) {  
		// if valid input file name
		if (argc == 2) { rd_file = argv[1]; }
		else {
			cout << "\n- HIVE simulator -";
			cout << "\nCommand line syntax is:";
			cout << "\nhive_sim";
			cout << "\nhive_sim <filename> - load *.hal file";
			return(-1); 
		}
	}

#ifdef MSWIN
	// size the MSWIN console
	con_size(CON_W, CON_H);
#else
	// switch POSIX kbd mode to polled
	kbd_mode(true, false);
#endif

	// set console font
	con_font('n');

	// create & init core
	core core;

	// file names
	const string help_fname = "hive_sim_help.txt";
	// command line
	const int32_t CMD_DEPTH = 32;  // cmd depth
	bool cmd_f = false;  // cmd flag
	bool err_f = false;  // cmd error flag
	string cmd_str = "cfg ";  // do config @ start
	string cmd_prev = "";  // cmd history
	string cmd_msg = "";  // cmd result message
	cmd_mem<CMD_DEPTH> cmd_hist;  // cmd buffer
	uint32_t cursor_pos = 0;
	//
	int32_t thd = 0; // thread 0 @ start
	string radix = "HEX";  // hex radix  @ start
	vector<string> vtoken;  // key code tokens
	uint32_t disp_addr = 0;  // addr at screen center
	bool first_f = true;  // do extra stuff @ start
	uint32_t f7_cy = 1;  // F7 cycles to do
	uint32_t f8_bp = 0;  // F8 breakpoint addr
	bool home_f = true;  // if true then disp_addr follows PC

	
	// loop forever
	while (true) {

		if (!first_f) { 

		    //////////////
		    // key proc //
		    //////////////

			// get key code
			uint64_t key_i = kbd_getkey();

			// do editing
			if (!cmd_edit(cmd_str, cursor_pos, key_i)) {
				// cmd hist recall
				if (key_i == KBD_UP) { if (cmd_hist.rd(cmd_str, true)) { cursor_pos = cmd_str.size(); } }
				else if (key_i == KBD_DN) { if (cmd_hist.rd(cmd_str, false)) { cursor_pos = cmd_str.size(); } }
				// select thread
				else if (key_i == KBD_F1) { thd = 1; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_F2) { thd = 2; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_F3) { thd = 3; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_F4) { thd = 4; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_SHFT_F1) { thd = 5; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_SHFT_F2) { thd = 6; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_SHFT_F3) { thd = 7; if (home_f) { disp_addr = core.pc_0[thd]; } }
				else if (key_i == KBD_SHFT_F4) { thd = 0; if (home_f) { disp_addr = core.pc_0[thd]; } }
				// do single clock
				else if (key_i == KBD_F5) { core.clk(); if (home_f) { disp_addr = core.pc_0[thd]; } }
				// do single cycle
				else if (key_i == KBD_F6) { core.cycle(); if (home_f) { disp_addr = core.pc_0[thd]; } }
				// do cycles
				else if (key_i == KBD_F7) { cmd_str += " cy "; }
				// do breakpoint
				else if (key_i == KBD_F8) { cmd_str += " bp "; }
				// dec thread
				else if (key_i == KBD_ALT_LT) { thd = (thd - 1) & THD_MSK; if (home_f) { disp_addr = core.pc_0[thd]; } }
				// inc thread
				else if (key_i == KBD_ALT_RT) { thd = (thd + 1) & THD_MSK; if (home_f) { disp_addr = core.pc_0[thd]; } }
				// listing line up
				else if (key_i == KBD_ALT_UP) { disp_addr = core.main_mem.calc_addr(disp_addr, -1); home_f = false; }
				// listing line down
				else if (key_i == KBD_ALT_DN) { disp_addr = core.main_mem.calc_addr(disp_addr, 1); home_f = false; }
				// listing page up
				else if (key_i == KBD_ALT_PGUP) { disp_addr = core.main_mem.calc_addr(disp_addr, -32); home_f = false; }
				// listing page down
				else if (key_i == KBD_ALT_PGDN) { disp_addr = core.main_mem.calc_addr(disp_addr, 32); home_f = false; }
				// listing home
				else if (key_i == KBD_ALT_HOME) { disp_addr = core.pc_0[thd]; home_f = true; }
			}
		}

		////////////////
		// token proc //
		////////////////

		if (!cmd_str.empty() && ch_is_white(cmd_str.back())) {
			vtoken = core.main_mem.str_to_tokens(cmd_str);
			int32_t tokens = vtoken.size();
			cmd_f = false;
			if (tokens) {
				// get last token
				string last_token = vtoken[tokens-1];
				// radix = hex
				if ((last_token == "h") && (tokens == 1)) { 
					cmd_f = true; 
					err_f = false; 
					radix = "HEX"; 
				}
				// radix = unsigned decimal
				else if ((last_token == "u") && (tokens == 1)) { 
					cmd_f = true; 
					err_f = false; 
					radix = "UNS"; 
				}
				// radix = signed decimal
				else if ((last_token == "i") && (tokens == 1)) { 
					cmd_f = true; 
					err_f = false; 
					radix = "INT"; 
				}
				// refresh screen
				else if ((last_token == "r") && (tokens == 1)) { 
					cmd_f = true; 
					err_f = false; 
					con_cls(); 
				}
				// set thread
				else if (last_token == "t") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting a thread"; }
					if (!err_f) {
						thd = str_to_int(vtoken[0], "AUTO") % THREADS;
						if (home_f) { disp_addr = core.pc_0[thd]; }
					}
				}
				// go to address
				else if (last_token == "g") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting an address"; }
					if (!err_f) {
						disp_addr = str_to_int(vtoken[0], "AUTO") & MEM_ADDR_MSK;
						home_f = false;
					}
				}
				// go home
				else if ((last_token == "home") && (tokens == 1)) { 
					cmd_f = true;
					err_f = false; 
					disp_addr = core.pc_0[thd];
					home_f = true;
				}
				// quit
				else if ((last_token == "quit") && (tokens == 1)) {
					cmd_f = true;
					err_f = false; 
					// restore orig keyboard mode
					#ifndef MSWIN
						kbd_mode(false, true);
					#endif
					return(0);
				}
				// config the core, optional input file name
				else if (last_token == "cfg") {
					cmd_f = true;
					err_f = (tokens > 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) {
						if (tokens == 2) { rd_file = vtoken[0];	}  // update file name
						con_cls();  // clear screen
						con_xy(0, 0); 
						err_f = core.cfg(rd_file, false, true);
						cout << endl << endl << " press any key to continue...";
						kbd_wait();
			        	if (home_f) { disp_addr = core.pc_0[thd]; }
			        	con_cls();  // clear screen
					}
				}
				// help
				else if ((last_token == "help") && (tokens == 1)) {
					cmd_f = true;
					err_f = view_file(help_fname, CON_H);  // call file viewer
					if (err_f) { cmd_msg = "can't read file:" + help_fname; }
					con_cls();  // clear screen
				}
				// do cycles
				else if (last_token == "cy") {
					cmd_f = true;
					err_f = (tokens > 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) {
						if (tokens == 2) { f7_cy = str_to_int(vtoken[0], "AUTO"); }
						cout << " cy running...";  // give visual indication
			        	for (uint32_t c = 0; c < f7_cy; c++) { core.cycle(); }
			        	if (home_f) { disp_addr = core.pc_0[thd]; }
					}
				}
				// run to breakpoint
				else if (last_token == "bp") {
					cmd_f = true;
					err_f = (tokens > 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) {
						if (tokens == 2) { f8_bp = str_to_int(vtoken[0], "AUTO"); }
						cout << " bp running...";  // give visual indication
						do { 
							core.cycle();
							if (kbd_hit()) break;
						} while (core.pc_0[thd] != f8_bp);
			        	if (home_f) { disp_addr = core.pc_0[thd]; }
					}
				}
				// clear all
				else if ((last_token == "cla") && (tokens == 1)) {
					cmd_f = true;
					core.cla_i = true;
				}
				// issue xsr, if no param then this thread
				else if (last_token == "xsr") {
					cmd_f = true;
					err_f = (tokens > 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) {
						if (tokens == 1) { core.xsr_i = 1 << thd; }
						else { core.xsr_i = 1 << str_to_int(vtoken[0], "AUTO"); }
					}
				}
				// issue isr, if no param then this thread
				else if (last_token == "isr") {
					cmd_f = true;
					err_f = (tokens > 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) {
						if (tokens == 1) { core.isr_i = 1 << thd; }
						else { core.isr_i = 1 << str_to_int(vtoken[0], "AUTO"); }
					}
				}
				// read stack top
				else if (last_token == "rs") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting stack"; }
					if (!err_f) { 
						int32_t stack = str_to_int(vtoken[0], "AUTO");
						err_f = (stack >= STACKS) | (stack < 0);
						if (err_f) { cmd_msg = "stack out of range"; }
						if (!err_f) { cmd_msg = int_to_str(core.stacks[thd].top(stack).val, radix, 32, true); }
					}
				}
				// write stack (pop & push)
				else if (last_token == "ws") {
					cmd_f = true;
					err_f = (tokens != 3);
					if (err_f) { cmd_msg = "expecting stack, data"; }
					if (!err_f) { 
						int32_t stack = str_to_int(vtoken[0], "AUTO");
						err_f = (stack >= STACKS) | (stack < 0);
						if (err_f) { cmd_msg = "stack out of range"; }
						if (!err_f) { 
							alu_t data;
							data.val = str_to_int(vtoken[1], "AUTO");
							core.stacks[thd].pop(stack);
							core.stacks[thd].psh(stack, data);
							core.stacks[thd].hist_upd();
						}
					}
				}
				// pop stack
				else if (last_token == "pop") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting stack"; }
					if (!err_f) { 
						int32_t stack = str_to_int(vtoken[0], "AUTO");
						err_f = (stack >= STACKS) | (stack < 0);
						if (err_f) { cmd_msg = "stack out of range"; }
						if (!err_f) { 
							core.stacks[thd].pop(stack);
							core.stacks[thd].hist_upd();
						}
					}
				}
				// push stack
				else if (last_token == "psh") {
					cmd_f = true;
					err_f = (tokens != 3);
					if (err_f) { cmd_msg = "expecting stack, data"; }
					if (!err_f) { 
						int32_t stack = str_to_int(vtoken[0], "AUTO");
						err_f = (stack >= STACKS) | (stack < 0);
						if (err_f) { cmd_msg = "stack out of range"; }
						if (!err_f) { 
							alu_t data;
							data.val = str_to_int(vtoken[1], "AUTO");
							core.stacks[thd].psh(stack, data);
							core.stacks[thd].hist_upd();
						}
					}
				}
				// read byte from mem
				else if (last_token == "rb") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting addr"; }
					if (!err_f) { cmd_msg = int_to_str(core.main_mem.rd(str_to_int(vtoken[0], "AUTO"), 1), radix, 8, true); }
				}
				// read half word from mem
				else if (last_token == "rh") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting addr"; }
					if (!err_f) { cmd_msg = int_to_str(core.main_mem.rd(str_to_int(vtoken[0], "AUTO"), 2), radix, 16, true); }
				}
				// read word from mem
				else if (last_token == "rm") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "expecting addr"; }
					if (!err_f) { cmd_msg = int_to_str(core.main_mem.rd(str_to_int(vtoken[0], "AUTO"), 4), radix, 32, true); }
				}
				// write byte to mem
				else if (last_token == "wb") {
					cmd_f = true;
					err_f = (tokens != 3);
					if (err_f) { cmd_msg = "expecting addr, data"; }
					if (!err_f) { core.main_mem.wr(str_to_int(vtoken[0], "AUTO"), 1, str_to_int(vtoken[1], "AUTO")); }
				}
				// write half word to mem
				else if (last_token == "wh") {
					cmd_f = true;
					err_f = (tokens != 3);
					if (err_f) { cmd_msg = "expecting addr, data"; }
					if (!err_f) { core.main_mem.wr(str_to_int(vtoken[0], "AUTO"), 2, str_to_int(vtoken[1], "AUTO")); }
				}
				// write word to mem
				else if (last_token == "wm") {
					cmd_f = true;
					err_f = (tokens != 3);
					if (err_f) { cmd_msg = "expecting addr, data"; }
					if (!err_f) { core.main_mem.wr(str_to_int(vtoken[0], "AUTO"), 4, str_to_int(vtoken[1], "AUTO")); }
				}
				// write to gpio
				else if (last_token == "wio") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) { core.io_rd_reg = str_to_int(vtoken[0], "AUTO"); }
				}
				// push rx uart
				else if (last_token == "wu") {
					cmd_f = true;
					err_f = (tokens != 2);
					if (err_f) { cmd_msg = "too many parameters"; }
					if (!err_f) { core.uart_rx_fifo.psh(str_to_int(vtoken[0], "AUTO")); }
				}
				// pop tx uart
				else if ((last_token == "ru") && (tokens == 1)) {
					cmd_f = true;
					err_f = false; 
					cmd_msg = int_to_str(core.uart_tx_fifo.top(), radix, 32, true);
					core.uart_tx_fifo.pop(); 
				}
				// pop lcd
				else if ((last_token == "rl") && (tokens == 1)) {
					cmd_f = true;
					err_f = false; 
					cmd_msg = int_to_str(core.lcd_fifo.top(), radix, 32, true);
					core.lcd_fifo.pop(); 
				}
				// do command line stuff
				if (cmd_f) { 
					cmd_prev = cmd_str;  // keep old
					cmd_hist.wr(cmd_str);  // store & clear cmd
					cursor_pos = cmd_str.size();  // reset cursor
					if (err_f) { cmd_msg = "<?> " + cmd_msg; }  
					else { cmd_msg = "<OK> " + cmd_msg; }
					cmd_prev += cmd_msg;  // keep old
					cmd_msg = "";  // clear
				}
			}
		}

		//////////
		// draw //
		//////////

		const int32_t PREV_Y = 49;
		const int32_t CMD_Y = PREV_Y + 1;
		const int32_t CMD_W = CON_W;

		// draw listing, stack & regs info
		core.main_mem.draw(0, 0, disp_addr, core.pc_0[thd], radix);
		core.draw_stack_info(44, 35, thd, radix);
		core.rbus_regs_draw(85, 35, radix);

		// draw state
		con_xy(0, 35); 
		con_font('i');
		cout << setw(8) << right << "PC";
		cout << setw(8) << right << "CY";
		cout << setw(8) << right << "RDX";
		cout << setw(16) << right << "THD";
		// 
		con_xy(0, 36); 
		con_font('n');
		cout << setw(8) << hex << showbase << right << core.pc_0[thd];
		cout << setw(8) << dec << right << core.time_2 / THREADS;
		cout << setw(8) << right << radix;
		cout << setw(16) << dec << right << thd;

		con_xy(0, 39); 
		con_font('i');
		cout << setw(8) << right << "BP";
		cout << setw(8) << right << "CY";
		cout << setw(24) << right << "File";
		// 
		con_xy(0, 40); 
		con_font('n');
		cout << setw(8) << hex << showbase << right << f8_bp;
		cout << setw(8) << dec << right << f7_cy;
		cout << setw(24) << right << core.main_mem.file[disp_addr];
		// 
		con_xy(0, 41); 
		con_font('n');
		cout << setw(40) << right << int_to_str(core.main_mem.crc, "HEX", 32, false) + ".ttl";

		// draw command line
		if (err_f) { con_font('e'); }  // hilite errors
		con_xy(0, PREV_Y); cout << setw(CMD_W) << "";  // clear & disp cmd_prev line
		con_xy(0, PREV_Y); cout << cmd_prev;
		con_font('n');
		con_xy(0, CMD_Y); cout << setw(CMD_W) << "";  // clear & disp cmd line
		con_xy(0, CMD_Y); cout << cmd_str;
		con_xy(cursor_pos, CMD_Y);  // position cursor

		// clear flag
		first_f = false;

	}
	
} // end of main

