# Compiler

## Author
- **Name:** Adam Jamrozinski

## Language: C++, bison & flex

---

## Files Included
- `compiler/CMakeLists.txt`: CMake configuration file for building the project.
- `compiler/parser.y`: bison parser.
- `compiler/parser.h`: bison header file output.
- `compiler/parser.cpp`: bison parser file output.
- `compiler/lexer.l`: flex scanner.
- `compiler/lexer.cpp`: flex scanner file output.
- `compiler/parser_utils.h`: header file with all additional utilities the parser relies on.
- `compiler/parser_utils.cpp`: all the implementations for the utilities.
- `compiler/math_module/multiplication.mr`: machine-readable implementation of multiplication algorithm.
- `compiler/math_module/division.mr`: implementation of division and modulo.
- `README.md`: This file, containing project description.
  
---


## Prerequisites
Before building the project, ensure the following tools are installed on your system:
- **CMake** (version 3.28 or higher)
- **C++ Compiler**:
    - Linux: GCC or Clang
- **C++ Standard**:
  - C++20
- **Build Tools**:
    - Linux: Make or Ninja
- **Flex** (min version 2.6.4)
- **Bison** (min version 3.8.2)
---
