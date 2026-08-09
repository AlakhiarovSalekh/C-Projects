# Arithmetic Compiler

A simple compiler that takes arithmetic expressions as input and generates assembly-like code. This project demonstrates the basics of compiler construction with lexical analysis, parsing, and code generation.

## Features

- Tokenizes arithmetic expressions
- Parses and evaluates expressions
- Generates assembly code representation
- Supports basic arithmetic operations (addition, subtraction, etc.)

## Building the Project

The project uses CMake for building:

```bash
cd arithmetic-compiler
mkdir build
cd build
cmake ..
make
```

## Usage

After building, run the compiler:

```bash
./ArithmeticCompiler
```

Enter an arithmetic expression when prompted, for example:
```
3 + 4 - 2
```

The program will display the result of the expression and generate corresponding assembly code.

## Project Structure

- `src/` - Source code files
  - `main.c` - Entry point
  - `tokenizer.c` - Lexical analysis
  - `parser.c` - Syntax analysis
  - `assembly.c` - Code generation
- `include/` - Header files
- `CMakeLists.txt` - CMake build configuration

## License

See the LICENSE file in the root directory for details.