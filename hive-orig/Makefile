# Evan Kahn 8/21/2023
# Run make from within cygwin to build on windows - easy setup for conio.h.
# todo: https://stackoverflow.com/questions/56457209/setting-up-vs-code-for-c-using-cygwin64-compiler-and-debugger-on-windows

# relative paths to file handles passed into the executable at compile time
# this should be replaced with something better in the future, but it helps
# separate the build outputs from the source files for version control

DEFINES += -DBIN_DIR=./
DEFINES += -DDOCS_DIR=../docs/
DEFINES += -DASM_DIR=../hive-asm/

SRC_DIR = hive-sim
HAL_DIR = hive-asm
BUILD_DIR = build
TARGET = hive_sim
BUILD_EXTS = *.ttl *.mif *.spi

.PHONY: all clean directory run
CC = g++

MKDIR = mkdir

LDFLAGS = 
INCFLAGS = 
EXE_SUFFIX =
ifeq ($(OS), Windows_NT)
	LDFLAGS = -static -static-libstdc++
	EXE_SUFFIX = .exe
	
	DEFINES += -DMSWIN
	LDFLAGS += -lwinmm -lws2_32
endif

CPPFLAGS = -Wall --std=gnu++11
MAINFILE = $(addprefix $(SRC_DIR)/, $(TARGET)).cpp
SRCFILES = $(SRC_DIR)/*.cpp
BIN_NAME = $(TARGET)$(EXE_SUFFIX)
MAINBIN = $(addprefix $(BUILD_DIR)/, $(BIN_NAME))

all: $(BUILD_DIR) $(MAINBIN)

$(BUILD_DIR):
	@-$(MKDIR) -p $(BUILD_DIR)

$(MAINBIN): $(MAINFILE) $(SRCFILES) $(BUILD_DIR)
	@echo Building $@
	@$(CC) $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS) $(DEFINES) -o $@

run: $(MAINBIN)
	@cd $(BUILD_DIR) && ./$(BIN_NAME)

clean:
	@-rm -rf $(BUILD_DIR)
