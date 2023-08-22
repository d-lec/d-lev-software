// ESK 8/21/2023
// here's a stripped down main function that only assembles the source files,
// rather than setting up the whole console interface.
// next step here is... get it running with a debugger
// so we can figure out where things go wrong on windows
// (or just get rid of all the platform specific dependencies - shouldn't be too hard)

// this gets passed into the core.cfg function
#define HAL_BUILD_QUIET false

// c++ stuff
//replacements for contents of bits/stdc++.h
#include <thread>
#include <sstream>
#include <fstream>
#include <string>
#include <bitset>
#include <unordered_set>
using namespace std;

// hive stuff
#include "hive_pkg.cpp"

// Including this allows it to compile under both windows and linux,
// as long as we're not actually calling these functions?
// Very ugly workaround but fine for now
#include "hive_console_linux.cpp"

#include "hive_cmd.cpp"
#include "hive_core.cpp"
#include "hive_view_file.cpp"

//////////
// MAIN //
//////////

int main(int argc, char* argv[]) {

	// default input file
	string rd_file = "theremin.hal";

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

	// create & init core
	core core;
	bool err_f = core.cfg(rd_file, HAL_BUILD_QUIET, true);
    cout <<"ded" <<endl;
    return (int)(err_f);
}
