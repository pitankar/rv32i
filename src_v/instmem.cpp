/**
 * MIT License
 * 
 * Copyright (c) 2020 Piyush Itankar <pitankar@gmail.com>
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <fstream>
#include <iostream>

#include "test_infra.h"
#include "Vinstmem.h"
#include "verilated.h"

using namespace std;

int main(int argc, char **argv, char **env) {
    Verilated::commandArgs(argc, argv);
    Vinstmem *instmem = new Vinstmem;
    
    int status = 0, address = 0, instruction;
    string line;
    ifstream progFile("program.txt");

    while (!progFile.eof()) {
        instmem->addr = address;
        instmem->eval();

        // Read a line from the program file
        getline(progFile, line);
        if (line.length() == 0)
            break;

        // Convert to hex if the line was no empty
        instruction = stoi(line, 0, 16);

        // Check if the memory has the same content as the file
        ASSERT((instmem->instr == instruction),
               status,
               terminate,
               "[%03d]: 0x%04x != 0x%04x \n", address, instmem->instr, instruction);

        address++;
    }

    printf("All Checks Passed!\n");

terminate:
    progFile.close();
    instmem->final();
    delete instmem;

    return status;
}