# Advanced Data Representation Utility

Author: Leandro

This is my second assignment of my Computer Org. & Architecture class.

This is a C program that handles advanced number conversions and signed number representations.  
It converts between octal, binary, and hexadecimal, and shows how numbers are stored in sign magnitude, one’s complement, and two’s complement.  
The file in this case is called **"a2_test.txt"**.

## Files in this folder

- `convert.c` — functions:
  - `oct_to_bin(octal_text, output)`
  - `oct_to_hex(octal_text, output)`
  - `hex_to_bin(hex_text, output)`
  - `to_sign_magnitude(number, output)`
  - `to_ones_complement(number, output)`
  - `to_twos_complement(number, output)`
  - (plus `div_convert`, `sub_convert`, `print_tables` from Assignment 1)

- `main.c` — reads test cases from a text file and prints PASS or FAIL,  
  and writes all results into **output.txt**

You should also have your test file:

- `a2_test.txt`

## Where to put the test file

Place the test file in the same folder as `main.c` and `convert.c`.  
When the program runs, it opens **a2_test.txt** from the current working folder.

Project layout example

- A dvancedDataRepresentation
  - convert.c
  - main.c
  - a2_test.txt // test file
  - CMakeLists.txt
  - (build files will appear in here after compiling)

## Notes

- Numbers in the test file can be written in decimal or in hexadecimal with `0x` in front.  
- The program uses only standard C headers.  
- If you change file names, also update `fopen("a2_test.txt", "r")` in `main.c`.  
- The program creates a file called **output.txt** containing all test results.


