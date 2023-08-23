// hive_view_file.cpp
// text file viewer
#pragma once


// text file => string vector
// return true if error
bool file_to_sv(const string& fname_i, vector<string>& sv_o) {
	ifstream rd_file;
	rd_file.open(STRING(DOCS_DIR) + fname_i, ios::in | ios::binary);  // open file stream
	if(!rd_file.is_open()) { return(true); }
	// read file lines to sv & close file
	vector<string> rd_sv;
	string line;
	while (getline(rd_file, line)) {
		sv_o.push_back(line);
	} 
	rd_file.close();
	return(false);
}


// view text file
// return true if error
bool view_file(const string& fname_i, int32_t conh_i) {
	// read file lines to sv & close file
	vector<string> rd_sv;
	if(file_to_sv(fname_i, rd_sv)) { return(true); }  // error
	int32_t line_min = 0;
	int32_t view_h = conh_i - 2;
	int32_t lines = rd_sv.size();
	bool done_f = false;
	while (!done_f) {
		con_cls();  // clear screen
		for (int32_t line = line_min; line < line_min+view_h; line++) {
			if (line < lines) { cout << rd_sv[line]; }
			cout << endl;
		}
		cout << endl << "- UP, DN, PGUP, PGDN, HOME, END - ESC to quit -";
		int64_t key = kbd_getkey();  // blocking
		if (key == KBD_ESC) { done_f = true; }
		else if (key == KBD_UP) { line_min--; }
		else if (key == KBD_DN) { line_min++; }
		else if (key == KBD_PGUP) { line_min -= view_h; }
		else if (key == KBD_PGDN) { line_min += view_h; }
		else if (key == KBD_HOME) { line_min = 0; }
		else if (key == KBD_END) { line_min = lines - view_h; }
		if (line_min < 0) { line_min = 0; }
		if (line_min > lines - view_h) { line_min = lines - view_h; }
	}
	return(false);
}
