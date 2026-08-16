# Enigma M4 Simulator

A simple and clean implementation of the Enigma Machine in C. 

More about Enigma M4: https://de.wikipedia.org/wiki/Enigma-M4

Datapath: SB -> W3 -> W2 -> W1 -> GrW -> UKW -> GrW -> W1 -> W2 -> W3 -> SB

## Usage

Enter plaintext/ciphertext at `./input.txt`

Set the configuration as strings: 
```C
char config[] = "I-IV-II:Beta:Bruno:VAAA:ANJV"; // W3-W2-W1:GrW:UKW:Ringstellung:Walzenstellung
char sb[] = "AT:BL:DF:GJ:HM:NW:OP:QY:RZ:VX"; // Steckbrett-Paar:...:...
```

## Build

Standard Mode:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Standard
cmake --build build
```

Show Internal Mode:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=ShowInternal
cmake --build build
```

If you don't have cmake:
```bash
gcc -o ./build/main ./src/*.c && ./build/main
```

## Run

```bash
./build/main
```