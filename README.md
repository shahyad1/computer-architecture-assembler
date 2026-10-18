# Computer Architecture Assembler (C)

A two-pass assembler coursework project implemented in C, with symbol-table support and sample assembly inputs.

## Project structure
- `Pass1/` — first-pass symbol-table processing
- `Pass2/` — second-pass translation
- `HashTable/` — hash-table implementation
- `Test/` — sample `.asm` inputs and `.mc` outputs

## Build
Requires CMake and a C compiler.

```bash
cmake -S . -B build
cmake --build build
```

The program expects an input and output filename and currently resolves files relative to `./Test/`. For example, from the repository root: `./build/assembler_project program.asm output.mc` (executable name/path may vary by generator).

## Language
C
