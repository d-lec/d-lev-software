# d-lev-software
This is a source mirror of the Hive compiler and the D-Lev theremin software.

# Instructions for use
Eric's original documentation lives in `hive_sim_help.txt` and `HCL_QUICK_START.txt`.

`.cpp` files build the Hive simulator / assembler. See `hive_sim_help.txt` for instructions on compiling and using it.
`.hal` files contain code that is built into the D-Lev software, written in assembly language for the Hive processor. It is assembled by `hive_sim` at launch.

The .mif and .spi binary outputs of `hive_sim` are two different encodings of the D-Lev software.
The `.mif` files are compiled into the FPGA image (see the [d-lev-hdl](https://github.com/d-lec/d-lev-hdl) repo), and describe a sort of "hardware bootloader" that runs by default on the FPGA; it loads the .spi file, and can also function in place of it should the user-replaceable EEPROM fail for any reason.
The `.spi` file is what actually gets loaded onto the external EEPROM through the [librarian](https://github.com/d-lec/d-lev-librarian).

TODO: Integrate the text documentation into this README (or a wiki), and replace it with a proper block diagram and/or flowchart.  

# Repo conventions
"Original text" for this repo can be found at http://d-lev.com/source/d-lev_sw_source_[RELEASE_DATE].zip.
When mirroring an update from the official D-Lev website, please switch to branch `eric-original`, make a "git tag" corresponding to the date of release in YYYY-MM-DD format, and upload the **original zip file from the website** to the Releases page.

