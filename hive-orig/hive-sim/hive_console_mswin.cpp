// hive_console_mswin.cpp
#pragma once

// colors: 0=black; 1=dark blue; 2=pine green; 3=aqua; 4=dark red; 5=purple; 6=khaki; 7=light gray;
//         8=medium gray; 9=royal blue; a=neon green; b=powder blue; c=red; d=majenta; e=light yellow; f=white;
const short CON_BG				= 0xF;  // background
const short CON_FG				= 0x3;  // foreground
const short CON_FB				= 0x0;  // foreground bold
const short CON_FE				= 0xC;  // foreground error
const short CON_BH				= 0x7;  // background hilite
const short CON_FH				= 0x2;  // foreground hilite
// derivations of basic params
const short CON_NORM			= (CON_BG << 4) | CON_FG;
const short CON_BOLD			= (CON_BG << 4) | CON_FB;
const short CON_ERROR			= (CON_BG << 4) | CON_FE;
const short CON_INV				= (CON_FG << 4) | CON_BG;
const short CON_INVBOLD			= (CON_FB << 4) | CON_BG;
const short CON_HILITE			= (CON_BH << 4) | CON_FH;
const short CON_BOLDHILITE		= (CON_BH << 4) | CON_FB;

// get console handle
const HANDLE CONHNDL = GetStdHandle(STD_OUTPUT_HANDLE);

// size the console
void con_size(short w_i, short h_i) {

	// get largest window possible
    COORD largest = GetLargestConsoleWindowSize(CONHNDL);
    
    // if OK, resize to requested
	if ((w_i <= largest.X) && (h_i <= largest.Y)) {
	    COORD size = {w_i, h_i};
	    SMALL_RECT rect = {0, 0, short(w_i-1), short(h_i-1)};
		SetConsoleScreenBufferSize(CONHNDL, size);
	    SetConsoleWindowInfo(CONHNDL, TRUE, &rect);
	}
	else {
		cout << "\nCannot set console to " << w_i << " width by " << h_i << " height!";
		cout << "\nSuggest you reduce the system console default font size";
		cout << "\nPress any key to continue...";
		getch();
	}
}

// returns concatenated bytes in kbd buffer
// blocking
uint64_t kbd_getkey() {
	uint64_t kcode = getch();  // concatenated key code
	if (kcode && (kcode != 0xE0)) {  // not esc (0 or 0xE0) : 1 byte code
		return(kcode);
	}
	else {  // else esc: 2 byte code
		kcode |= (getch() << 8);  // concat
		return(kcode);
	}
}

// non-blocking
bool kbd_hit() {
	if (kbhit()) { kbd_getkey(); return(true); }
	else { return(false); }
}

// returns concatenated bytes in kbd buffer
// non-blocking
uint64_t kbd_rdbuf() {
	if (kbhit()) { return(kbd_getkey()); }
	else { return(0); }
}

// blocking
void kbd_wait() {
	kbd_getkey();
	return;
}

// clear screen
void con_cls() { 
	// the "evil" Windows way:
	system("cls"); 
}

// position console cursor
void con_xy(short x_i, short y_i) {
    COORD pos = {x_i, y_i};
    SetConsoleCursorPosition(CONHNDL, pos) ;
}  

// set console font color
void con_font(char font_i) {
	switch(font_i) {
		case 'b' : SetConsoleTextAttribute(CONHNDL, CON_BOLD); break;  // bold
		case 'e' : SetConsoleTextAttribute(CONHNDL, CON_ERROR); break;  // error
		case 'h' : SetConsoleTextAttribute(CONHNDL, CON_HILITE); break;  // hilite
		case 'o' : SetConsoleTextAttribute(CONHNDL, CON_BOLDHILITE); break;  // bold hilite
		case 'i' : SetConsoleTextAttribute(CONHNDL, CON_INV); break;  // inverse
		case 's' : SetConsoleTextAttribute(CONHNDL, CON_INVBOLD); break;  // inverse bold
		case 'n' : SetConsoleTextAttribute(CONHNDL, CON_NORM); break;  // normal
		default  : SetConsoleTextAttribute(CONHNDL, CON_NORM); break;  // default
	}
}  

