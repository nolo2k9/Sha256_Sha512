# SHA 512 Digest
***
## Keith Nolan: G00351932
***
## Project Goal
- This program calculates the SHA512 value of an input file. 
- It takes the name or path of the file as a command line argument and outputs the SHA512 digest of it.
- This programme only runs with gcc or clang compilers

***

## This repository contains
- README.md - Describes the project and how you can set it up.
- Files Containing the preprocessing of Sha512
- The numerous functions used execute the Sha512 hashing algorithm
- A file containing the Sha512 implementation, incorporates all of the building blocks. 
- Make File
***
## How to run this project

### Installing necessities
#### Windows
- Install **Windows** subsystem for linux. Available at: https://docs.microsoft.com/en-us/windows/wsl/install-win10
***
- Download this repository from **Github**.
- Move to the project directory
- Move into 512 directory
- Make the file: make SHA512
- Run the following: ./Sha512 name or path of the file you want to digest
- Enjoy
***

### About Sha512


### Theory of Algorithms 2021 Workings

| Decimal | Hex | Binary |
|---------|-----|--------|
|       0 |   0 |   0000 |
|       1 |   1 |   0001 |
|       2 |   2 |   0010 |
|       3 |   3 |   0011 |
|       4 |   4 |   0100 |
|       5 |   5 |   0101 |
|       6 |   6 |   0110 |
|       7 |   7 |   0111 |
|       8 |   8 |   1000 |
|       9 |   9 |   1001 |
|      10 |   A |   1010 |
|      11 |   B |   1011 |
|      12 |   C |   1100 |
|      13 |   D |   1101 |
|      14 |   E |   1110 |
|      15 |   F |   1111 |

|Decimal | Hex |   Binary |
|--------|-----|----------|
|    241 |  F1 | 11110001 |
|    170 |  AA | 10101010 |
### References

##### Secure hash standard:
[1] https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf

##### A Peer-to-Peer Electronic Cash System (uses Sha256)
[2] Satoshi Nakamoto: Bitcoin: “A Peer-to-Peer Electronic Cash System“

##### Multi -line editing
[3] https://stackoverflow.com/questions/30037808/multiline-editing-in-visual-studio-code

##### Bitwise operations
[4] https://en.wikipedia.org/wiki/Bitwise_operations_in_C

##### Bitwise operators in C Programming
[5] https://www.programiz.com/c-programming/bitwise-operators

##### Declare 64 bit integers in C
[6] https://stackoverflow.com/questions/9606455/how-to-specify-64-bit-integers-in-c

##### Multi-line editing vscode
[7] https://kencenerelli.wordpress.com/2018/03/25/visual-studio-code-multi-line-and-multi-cursor-editing/

##### Writing endian independent code
[8] https://developer.ibm.com/technologies/systems/articles/au-endianc/

##### Byteswap
[9] https://stackoverflow.com/questions/41770887/cross-platform-definition-of-byteswap-uint64-and-byteswap-ulong

##### Unsigned 128 
[10] https://stackoverflow.com/questions/16088282/is-there-a-128-bit-integer-in-gcc

##### Command line arguments in C
[11] https://www.cprogramming.com/tutorial/c/lesson14.html

