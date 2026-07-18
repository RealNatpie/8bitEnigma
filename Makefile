# Makefile for EnigamTest
# Native Linux builds use gcc.
# Windows console builds use MinGW-w64 cross-compilers.

CC = gcc
CFLAGS = -Wall -Wextra
BIN_DIR = bin
TARGET = $(BIN_DIR)/linux/ENIGMA
COMMON_SRCS = rotors.c rotor_constants.c config_parser.c plugboard.c alphabet.c machine.c wiring.c
LINUX_SRCS = Linux/LinuxEnigma.c $(COMMON_SRCS)
DOS286_SRCS = DOS286/DOS286Enigma.c $(COMMON_SRCS)
CX16_SRCS = CBM/CX16Enigma.c $(COMMON_SRCS)
C64_SRCS = CBM/C64Enigma.c $(COMMON_SRCS)
APPLE2_SRCS = CBM/apple2Enigma.c $(COMMON_SRCS)
WIN_SRCS = windows/WinEnigma.c $(COMMON_SRCS)
ZCC ?= z88dk.zcc
CL65 ?= cl65
CPM_SRCS = CPM/CPMEnigma.c CPM/CPMText.c $(COMMON_SRCS)
CPM_TARGET = $(BIN_DIR)/cpm/ENIGMA
CPM_OUTPUT = $(CPM_TARGET).COM
CX16_OUTPUT = $(BIN_DIR)/cx16/x16Enigma.prg
C64_OUTPUT = $(BIN_DIR)/c64/c64Enigma.prg
APPLE2_OUTPUT = $(BIN_DIR)/apple2/apple2Enigma.bin
WIN_ARCH ?= amd64
MINGW_CC_amd64 ?= $(shell command -v x86_64-w64-mingw32-gcc 2>/dev/null || command -v x86_64-w64-mingw32-gcc-posix 2>/dev/null || command -v x86_64-w64-mingw32-gcc-win32 2>/dev/null)
MINGW_CC_x86 ?= $(shell command -v i686-w64-mingw32-gcc 2>/dev/null || command -v i686-w64-mingw32-gcc-posix 2>/dev/null || command -v i686-w64-mingw32-gcc-win32 2>/dev/null)
OWCC ?= open-watcom.owcc
OWROOT ?= /snap/open-watcom/current
DOS286_OUTPUT ?= $(BIN_DIR)/dos286/ENIGMA.EXE
# Open Watcom owcc flags for a DOS EXE targeting 80286.
# Override DOS286_FLAGS at invocation time if your toolchain uses different switches.
DOS286_FLAGS ?= -b dos -march=i286
DOS286_INCLUDES ?= -I$(OWROOT)/h

ifeq ($(WIN_ARCH),amd64)
WIN_CC := $(MINGW_CC_amd64)
WIN_OUTPUT := $(BIN_DIR)/windows/enigma-amd64.exe
else ifeq ($(WIN_ARCH),x64)
WIN_CC := $(MINGW_CC_amd64)
WIN_OUTPUT := $(BIN_DIR)/windows/enigma-amd64.exe
else ifeq ($(WIN_ARCH),x86)
WIN_CC := $(MINGW_CC_x86)
WIN_OUTPUT := $(BIN_DIR)/windows/enigma-x86.exe
else ifeq ($(WIN_ARCH),win32)
WIN_CC := $(MINGW_CC_x86)
WIN_OUTPUT := $(BIN_DIR)/windows/enigma-x86.exe
else
$(error Unsupported WIN_ARCH '$(WIN_ARCH)'; use x86 or amd64)
endif

.DEFAULT_GOAL := help

.PHONY: help all linux clean cpm clean-cpm cx16 c64 apple2 windows win32 win64 dos286 check-owcc check-wincc list-sources

help:
	@echo "Available targets:"
	@echo "  all      Build all platform outputs (linux, win32, win64, cpm, cx16, c64, apple2, dos286)"
	@echo "  linux    Build native Linux binary ($(TARGET))"
	@echo "  windows  Build Windows .exe (set WIN_ARCH=x86 or amd64)"
	@echo "  win32    Build 32-bit Windows .exe"
	@echo "  win64    Build 64-bit Windows .exe"
	@echo "  cpm      Build CP/M .COM output ($(CPM_OUTPUT))"
	@echo "  cx16     Build Commander X16 .prg output ($(CX16_OUTPUT))"
	@echo "  c64      Build Commodore 64 .prg output ($(C64_OUTPUT))"
	@echo "  apple2   Build Apple II binary output ($(APPLE2_OUTPUT))"
	@echo "  dos286   Build DOS 80286 .exe with Open Watcom (owcc)"
	@echo "  list-sources Print source files used by each target"
	@echo "  clean    Remove generated binaries"
	@echo "  clean-cpm Remove generated CP/M outputs"

list-sources:
	@echo "COMMON_SRCS: $(COMMON_SRCS)"
	@echo "LINUX_SRCS:  $(LINUX_SRCS)"
	@echo "WIN_SRCS:    $(WIN_SRCS)"
	@echo "CPM_SRCS:    $(CPM_SRCS)"
	@echo "CX16_SRCS:   $(CX16_SRCS)"
	@echo "C64_SRCS:    $(C64_SRCS)"
	@echo "APPLE2_SRCS: $(APPLE2_SRCS)"
	@echo "DOS286_SRCS: $(DOS286_SRCS)"

all: linux win32 win64 cpm cx16 c64 apple2 dos286

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

linux: $(TARGET)
	@cp enigma.cfg $(BIN_DIR)/linux/

$(TARGET): $(LINUX_SRCS)
	@mkdir -p $(BIN_DIR)/linux
	$(CC) $(CFLAGS) -o $(TARGET) $(LINUX_SRCS)

windows: $(WIN_OUTPUT)

check-wincc:
	@if [ -z "$(WIN_CC)" ] || ! command -v "$(WIN_CC)" >/dev/null 2>&1; then \
		echo "Error: Windows cross-compiler not found for WIN_ARCH=$(WIN_ARCH)."; \
		echo "Tried compiler: $(WIN_CC)"; \
		echo "Install MinGW-w64 (Ubuntu/Debian):"; \
		echo "  sudo apt install gcc-mingw-w64-i686 gcc-mingw-w64-x86-64"; \
		echo "Or override manually:"; \
		echo "  make windows WIN_ARCH=$(WIN_ARCH) MINGW_CC_$(WIN_ARCH)=/path/to/compiler"; \
		echo "Examples:"; \
		echo "  make win32 MINGW_CC_x86=i686-w64-mingw32-gcc"; \
		echo "  make win64 MINGW_CC_amd64=x86_64-w64-mingw32-gcc"; \
		exit 127; \
	fi

$(WIN_OUTPUT): check-wincc $(WIN_SRCS)
	@mkdir -p $(BIN_DIR)/windows
	$(WIN_CC) $(CFLAGS) -o $(WIN_OUTPUT) $(WIN_SRCS)
	@cp enigma.cfg $(BIN_DIR)/windows/

win32:
	$(MAKE) windows WIN_ARCH=x86

win64:
	$(MAKE) windows WIN_ARCH=amd64

check-owcc:
	@if ! command -v $(OWCC) >/dev/null 2>&1; then \
		echo "Error: $(OWCC) not found in PATH."; \
		echo "Install Open Watcom and/or run: make dos286 OWCC=/path/to/owcc"; \
		exit 127; \
	fi

dos286: check-owcc $(DOS286_OUTPUT)

$(DOS286_OUTPUT): $(DOS286_SRCS)
	@mkdir -p $(BIN_DIR)/dos286
	$(OWCC) $(CFLAGS) $(DOS286_FLAGS) $(DOS286_INCLUDES) -o $(DOS286_OUTPUT) $(DOS286_SRCS)
	@cp enigma.cfg $(BIN_DIR)/dos286/

cpm: $(CPM_OUTPUT)

$(CPM_OUTPUT): $(CPM_SRCS)
	@mkdir -p $(BIN_DIR)/cpm
	$(ZCC) +cpm -clib=8080 -create-app -o $(CPM_TARGET) $(CPM_SRCS)
	if [ -f $(CPM_TARGET).com ]; then \
		mv -f $(CPM_TARGET).com $(CPM_OUTPUT); \
	fi
	cp -r CPM/data/* $(BIN_DIR)/cpm/ 2>/dev/null || true
	@cp enigma.cfg $(BIN_DIR)/cpm/

cx16: $(CX16_OUTPUT)
	@cp enigma.cfg $(BIN_DIR)/cx16/

$(CX16_OUTPUT): $(CX16_SRCS)
	@mkdir -p $(BIN_DIR)/cx16
	$(CL65) -t cx16 -o $(CX16_OUTPUT) $(CX16_SRCS)

c64: $(C64_OUTPUT)
	@cp enigma.cfg $(BIN_DIR)/c64/

$(C64_OUTPUT): $(C64_SRCS)
	@mkdir -p $(BIN_DIR)/c64
	$(CL65) -t c64 -o $(C64_OUTPUT) $(C64_SRCS)

apple2: $(APPLE2_OUTPUT)
	@cp enigma.cfg $(BIN_DIR)/apple2/

$(APPLE2_OUTPUT): $(APPLE2_SRCS)
	@mkdir -p $(BIN_DIR)/apple2
	$(CL65) -t apple2 -o $(APPLE2_OUTPUT) $(APPLE2_SRCS)

clean-cpm:
	rm -rf $(BIN_DIR)/cpm

clean:
	rm -rf $(BIN_DIR)/linux $(BIN_DIR)/windows $(BIN_DIR)/dos286 $(BIN_DIR)/cpm $(BIN_DIR)/cx16 $(BIN_DIR)/c64 $(BIN_DIR)/apple2
