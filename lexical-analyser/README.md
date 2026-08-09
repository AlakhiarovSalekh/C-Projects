# Lexical Analyzer

A simple lexical analyzer (lexer) for C-like languages written in C. This tool breaks down source code into tokens, which is the first phase of a compiler.

## Overview

This lexical analyzer identifies and categorizes the following token types:
- Keywords (like `int`, `while`, `if`, etc.)
- Identifiers (variable names)
- Integers
- Operators (like `+`, `-`, `*`, `/`, etc.)
- Delimiters (like `{`, `}`, `(`, `)`, etc.)

## Features

- Recognizes 32 C keywords
- Validates identifier names according to C rules
- Identifies operators and delimiters
- Recognizes integer literals

## How to Build

Use the provided Makefile to build the lexer:

```bash
make
```

This will compile the source code and create an executable named `lexer`.

## How to Run

After building, you can run the lexical analyzer with:

```bash
make run
```

Or directly:

```bash
./lexer
```

## Example Output

For the input `int a = b + c`, the lexer will output:

```
For Expression "int a = b + c":
Token: Keyword, Value: int
Token: Identifier, Value: a
Token: Operator, Value: =
Token: Identifier, Value: b
Token: Operator, Value: +
Token: Identifier, Value: c
```

## How It Works

1. The lexer reads the input string character by character
2. It identifies token boundaries using delimiters
3. Each token is classified based on its pattern
4. The token type and value are then printed to the console

## Modifying the Input

To analyze different code snippets, modify the `lex_input` and `lex_input01` arrays in the `main()` function of `lexer.c`.

## License

See the LICENSE file in the root directory for licensing information.