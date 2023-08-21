// hive_console.cpp
#pragma once

#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <termios.h>
using namespace std;


// sleep for milliseconds
void ms_sleep(uint32_t ms_i) {
	this_thread::sleep_for(chrono::milliseconds(ms_i));
	return;
}

// kbd mode switcher
// new_i=true   : switch to new mode
// new_i=false  : switch back to old mode
// block_i=true : blocking reads
void kbd_mode(bool new_i, bool block_i){
	static bool first_f = true;
	static termios t_old = {0};
	if (first_f) {
		tcgetattr(STDIN_FILENO, &t_old);  // get old @ start
		first_f = false;
	}
	if (new_i) {
		termios t_new = t_old;  // copy old mode to modify
		t_new.c_lflag &= ~ICANON;  // turn off canonical form
		t_new.c_lflag &= ~ECHO;  // turn off echo
		t_new.c_cc[VMIN] = (block_i) ? 1 : 0;  // min bytes
		t_new.c_cc[VTIME] = 0;  // 0/10 sec inter byte time
		tcsetattr(STDIN_FILENO, TCSANOW, &t_new);  // set new mode
	}
	else { 
		tcsetattr(STDIN_FILENO, TCSANOW, &t_old);  // set orig mode
	} 
	return;
}

// returns concatenated bytes in kbd buffer (big endian)
// needs kbd_mode(true, false) to work
// non-blocking
int64_t kbd_rdbuf() {
	char rdbuf[8] = {0};  // 8 byte read buffer
	int64_t kcode = 0;  // concatenated key code
    int32_t bytes = read(STDIN_FILENO, rdbuf, 8);  // read up to 8 bytes
	for (int32_t idx = 0; idx < bytes; idx++) {
		kcode = ((kcode << 8) | uint64_t(rdbuf[idx]));  // concat
	}
	return(kcode);
}

// returns concatenated bytes in kbd buffer (big endian)
// delay of 1.0 ms dramatically reduces cpu load
// needs kbd_rdbuf to work
// blocking, internally polled
int64_t kbd_getkey() {
	cout << flush;  // output any pending chars
	int64_t kcode = kbd_rdbuf();
	while (!kcode) {  // poll until key press
		ms_sleep(1);  // 1 ms delay
		kcode = kbd_rdbuf();
	}
	return(kcode);
}

// non-blocking
bool kbd_hit() {
	return(kbd_rdbuf());
}

// wait until key hit
// blocking, internally polled
void kbd_wait() {
	cout << flush;  // output any pending chars
	while (!kbd_rdbuf()) { ms_sleep(1); }
	return;
}

// clear screen
void con_cls() { 
//	cout << "\033[2J";  // clears by scrolling down
	cout << "\033c";  // clears w/ no scrolling
}

// position console cursor
void con_xy(short x_i, short y_i) {
	cout << "\033[" << ++y_i << ";" << ++x_i << "H";
}

// set console font color
// attributes: 0=norm, 1=bold, 5=blink, 7=reverse
// foreground: 30=black, 31=red, 32=green, 33=yellow, 34=blue, 35=magenta, 36=cyan, 37=gray
// background: 40=black, 41=red, 42=green, 43=yellow, 44=blue, 45=magenta, 46=cyan, 47=gray, 48=white(?)
void con_font(char font_i) {
	switch(font_i) {
		case 'b' : cout << "\033[1;30;48m"; break;  // (bold, fg_blk, bg_wht) - bold
		case 'e' : cout << "\033[1;31;48m"; break;  // (bold, fg_red, bg_wht) - error
		case 'h' : cout << "\033[0;34;47m"; break;  // (norm, fg_blu, bg_gry) - hilite
		case 'o' : cout << "\033[1;30;47m"; break;  // (bold, fg_blk, bg_gry) - hilite bold
		case 'i' : cout << "\033[1;37;44m"; break;  // (bold, fg_gry, bg_blu) - inverse
		case 's' : cout << "\033[1;37;40m"; break;  // (bold, fg_gry, bg_blk) - inverse bold
		case 'n' : cout << "\033[0;34;48m"; break;  // (norm, fg_blu, bg_wht) - normal
		default  : cout << "\033[0;30;48m"; break;  // (norm, fg_blk, bg_wht) - default
	}
} 

