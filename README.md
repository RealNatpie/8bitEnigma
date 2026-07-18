# 8bit Enigma

## Overview

8bit Enigma is an emulator of the classic Enigma encryption device. It is designed to support multiple Enigma machine variants through text-editable configuration files (`.cfg`). The project specifically targets older 8-bit hardware platforms while maintaining compatibility with modern systems.

## Features

- **Configurable Machine Types**: Support for multiple Enigma variants via `enigma.cfg`
- **Custom Alphabets**: Define custom character sets beyond standard Latin alphabets
- **Multiple Rotor Support**: Configurable rotor wirings, notches, and positions
- **Reflector Variants**: Support for different reflector types (including rotating and rewirable reflectors)
- **Plugboard Support**: Full plugboard (Steckerbrett) implementation
- **Cross-Platform**: Single codebase compiled for multiple target systems

## Supported Platforms

### Modern Systems

- **Windows** (32-bit and 64-bit)
- **Linux** (native)
- **Commander X16**

### Legacy 8-bit Systems

- **CP/M** (Altair 8800 and compatible systems)
- **Commodore 64**
- **Apple II**
- **MS-DOS** (286-based systems)

### Future Planned Systems

- Amiga
- Commodore PET
- Atari 400/800
- ZX81 / Timex 1000
- ZX Spectrum

**Note**: Due to the reconfigurable emulation design, 8bit Enigma initially targets systems with disk drives and file systems to support external configuration files.

## Building

The project uses cross-compilers to maximize code reuse across platforms while avoiding period-specific compiler idiosyncrasies.

### Toolchains

| Platform | Compiler | Build Target |
| -------- | -------- | ------------ |
| Linux | gcc | `make linux` |
| Windows (32-bit) | MinGW (i686) | `make win32` |
| Windows (64-bit) | MinGW (x86_64) | `make win64` |
| Commander X16 | cc65 | `make cx16` |
| Commodore 64 | cc65 | `make c64` |
| Apple II | cc65 | `make apple2` |
| MS-DOS (286) | Open Watcom | `make dos286` |
| CP/M | z88dk | `make cpm` |

### Build Commands

```bash
# Build for current platform (Linux)
make linux

# Build for all platforms
make all

# Build specific platform
make win32
make win64
make cpm
make cx16
make c64
make apple2
make dos286

# Clean build artifacts
make clean

# View available targets
make help
```

### Prerequisites

- **gcc** (for Linux builds)
- **MinGW-w64** (for Windows cross-compilation)
- **cc65** (for Commodore 64, Commander X16, and Apple II)
- **Open Watcom** (for DOS 286)
- **z88dk** (for CP/M)

## Project Structure

```text
8bitEnigma/
├── Core Engine
│   ├── machine.c/h          - Enigma machine implementation
│   ├── rotors.c/h           - Rotor logic and stepping
│   ├── plugboard.c/h        - Plugboard (Steckerbrett) implementation
│   ├── alphabet.c/h         - Custom alphabet support
│   ├── wiring.c/h           - Wiring mappings
│   └── rotor_constants.c/h  - Rotor definitions
├── Configuration
│   ├── config_parser.c/h    - Configuration file parser
│   ├── parser_errors.h      - Error definitions
│   └── enigma.cfg           - Machine configuration file
├── Platform Ports
│   ├── Linux/               - Linux console application
│   ├── windows/             - Windows console application
│   ├── CBM/                 - Commodore systems (C64, CX16)
│   ├── CPM/                 - CP/M implementation
│   └── DOS286/              - MS-DOS 286 version
└── Build System
    └── Makefile             - Cross-platform build system
```

## Configuration Format

The `enigma.cfg` file defines:

- **Alphabets**: Custom character sets (e.g., 26-letter English)
- **Rotors**: Wiring patterns, notch positions, and turnover behavior
- **Machines**: Complete Enigma variants with rotor selections and reflectors
- **Reflectors**: Fixed or rotating reflector configurations

Example configuration structure:

```ini
[ALPHABETS]
NewAlphabet
Id=1
Name=Standard English
Size=26
SymbolMap=ABCDEFGHIJKLMNOPQRSTUVWXYZ

[ROTORS]
NewRotor
Id=1
Name=Rotor I
AlphabetId=1
Wiring=EKMFLGDQVZNTOWYHXUSPAIBRCJ
Notches=Q

[MACHINES]
NewMachine
Id=1
Name=Enigma M3
...
```

## Usage

1. Configure your Enigma machine settings in `enigma.cfg`
2. Run the platform-specific executable
3. Enter text to encrypt/decrypt

## Development Status

This project is currently **in active development**. Core functionality is implemented and compiles for all target platforms. Testing and platform-specific optimizations are ongoing.

## License

[Include license information here]

## Contributing

Contributions are welcome! This project aims to preserve the functionality of historical Enigma machines while making them accessible on vintage computing platforms.

## Acknowledgments

This project recreates the functionality of the historical Enigma cipher machines for educational and preservation purposes.
