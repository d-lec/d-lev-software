# Evan Kahn 8/21/2023

SRC_DIR = hive-sim
HAL_DIR = hive-asm
BUILD_DIR = build
TARGET = hive_sim

.PHONY: all clean directory run
CC = g++

MKDIR = mkdir
DEFINES =
EXE_SUFFIX =
ifneq ($(OS), Windows_NT)
	MKDIR += -p
else
	EXE_SUFFIX = .exe
endif

CPPFLAGS = --std=c++11
MAINFILE = $(addprefix $(SRC_DIR)/, $(TARGET)).cpp
SRCFILES = $(SRC_DIR)/*.cpp
MAINBIN = $(addprefix $(BUILD_DIR)/, $(TARGET))$(EXE_SUFFIX)

all: directory $(MAINBIN)

$(MAINBIN): $(MAINFILE) $(SRCFILES)
	@echo Building $@
	@$(CC) $< $(CPPFLAGS) $(DEFINES) -o $@

directory:
	@$(MKDIR) -p $(BUILD_DIR)

clean:
	@-rm -rf $(BUILD_DIR)
