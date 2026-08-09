# QR Code Generator

A simple command-line tool that generates QR codes from web links. You can display QR codes directly in your terminal or save them as PNG images.

## Quick Start

```bash
# Install dependencies (Ubuntu/Debian)
make install-deps

# Build the project
make

# Generate QR code and show in terminal
./bin/qr_gen "https://devlogs.xyz"

# Generate QR code and save as PNG
./bin/qr_gen "https://devlogs.xyz" my_qr_code.png
```

## Project Structure

```
qr_code_project/
├── bin/           # Executable output
├── obj/           # Object files
├── qr_code.h      # Header file
├── main.c         # Main program
├── png_writer.c   # PNG generation
├── display.c      # Terminal display
└── Makefile       # Build automation
```

## Requirements

- `libqrencode`
- `libpng`

### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get install libqrencode-dev libpng-dev
```

**macOS:**
```bash
brew install qrencode libpng
```

## Build

### With Anvil

If you have [Anvil](https://github.com/dexter-xD/Anvil) build system installed, you can build using the `anvil.conf` file:

```bash

# build
anvil anvil.conf
cd build && make run

# or watch mode (auto-rebuild on file changes)
anvil -w anvil.conf

# or watch & run mode (auto-rebuild and execute)
anvil -wr anvil.conf

```
This creates the `qr_gen` executable in the `build/bin/` directory.
The configuration is already set up in `anvil.conf` for easy building.

### Using Makefile
```bash
# Build the project (This creates the `qr_gen` executable in the `bin/` directory.)
make

# Build and run example
make run

# Clean build files
make clean

# Install dependencies (Ubuntu/Debian)
make install-deps
```

### Manual Compilation
```bash
gcc -o bin/qr_gen main.c png_writer.c display.c -lqrencode -lpng
```

## Usage

```bash
./bin/qr_gen <url> [output.png]
```

- `<url>` - Website link to encode in QR code
- `[output.png]` - Optional: Save as PNG file (shows in terminal if omitted)

## Examples

```bash
# Show QR code in terminal
./bin/qr_gen "https://devlogs.xyz"

# Save QR code as image
./bin/qr_gen "https://devlogs.xyz" qr_code.png
```

## Features

- Generate QR codes from any URL
- Display in terminal using Unicode blocks
- Export as high-quality PNG images
