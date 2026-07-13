# Enigma M4 Simulator

A simple and clean implementation of the Enigma Machine in C. 


## Requirements

- CMake
- A C23-compatible compiler

## Build

### Standard

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Standard
cmake --build build
```

### Show Internal

```bash
cmake -B build -DCMAKE_BUILD_TYPE=ShowInternal
cmake --build build
```

## Run

```bash
./build/main
```