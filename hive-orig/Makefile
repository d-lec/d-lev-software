# Evan Kahn 8/21/2023
# This is unfinished but hopefully it's a good start.
# I'm trying to do hacky workarounds in the makefile for things that are easy enough to just fix in the code.
# Next time I look at this, that's what to do next.
# also, this: https://code.visualstudio.com/docs/cpp/config-mingw

SRC_DIR = hive-sim
HAL_DIR = hive-asm
BUILD_DIR = build
TARGET = hive_sim
BUILD_EXTS = *.ttl *.mif *.spi

.PHONY: all clean directory run
CC = g++

MKDIR = mkdir

# Need this on windows otherwise the app will try to link to cygwin dylibs
LDFLAGS = -static -static-libstdc++
INCFLAGS = 
EXE_SUFFIX =
ifneq ($(OS), Windows_NT)
	DIRSEP = /
else
	DIRSEP = \\
	EXE_SUFFIX = .exe
	
	# we're looking for conio.h here but it doesn't quite work
	# need to figure out what toolchain eric was using
	# is it just msvc compilers?
	#
	# Update: it's cygwin.

	DEFINES = -DMSWIN
	LDFLAGS += -lwinmm -lws2_32
endif

CPPFLAGS = -Wall --std=gnu++11
MAINFILE = $(addprefix $(SRC_DIR)/, $(TARGET)).cpp
SRCFILES = $(SRC_DIR)/*.cpp
MAINBIN = $(addprefix $(BUILD_DIR)$(DIRSEP), $(TARGET))$(EXE_SUFFIX)

all: run

$(BUILD_DIR):
	@-$(MKDIR) -p $(BUILD_DIR)

$(MAINBIN): $(MAINFILE) $(SRCFILES) $(BUILD_DIR)
	@echo Building $@
	@$(CC) $< $(CPPFLAGS) $(INCFLAGS) $(LDFLAGS) $(DEFINES) -o $@

run: $(MAINBIN)
	@-cd $(HAL_DIR) && ..$(DIRSEP)$(MAINBIN)
	# Move firmware builds out of hive-asm directory and into build/
	@-mv $(HAL_DIR)/*.spi $(BUILD_DIR)
	@-mv $(HAL_DIR)/*.mif $(BUILD_DIR)
	@-mv $(HAL_DIR)/*.ttl $(BUILD_DIR)

clean:
	@-rm -rf $(BUILD_DIR)
