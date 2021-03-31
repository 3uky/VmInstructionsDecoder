#ifndef _CORE_HPP
#define _CORE_HPP

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdexcept>
#include <list>
#include <chrono>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <time.h>
#include <bitset>

#include "opcodes.hpp"

typedef struct {
    /* Here will be stored a data, that program is needed */
    VM_BYTE *dataArray;
    /* Here will be stack */
    VM_QWORD stackArray[STACK_SIZE];
    /* Here will be stored a program's code */
    VM_BYTE *codeArray;
} ADDRESS_SPACE, *PADDRESS_SPACE;

typedef struct {
    /* Here will be stored all registers */
    VM_QWORD regArray[REGS_COUNT];
    /* Program Counter */
    VM_QWORD pc;
    /* Stack Pointer */
    VM_QWORD sp;
} REGISTERS, *PREGISTERS;

class VirtualMachineModel {
    private:
    // HEADER, MEMORY, REGISTERS, VM FLOW
        bool isVMwork;
        bool isMainThread;
        PREGISTERS registers;
        PADDRESS_SPACE memory;
        PHEADER header;
    // MEMORY CONTROLLERS
        VM_BYTE maskBitStream;
        std::vector<VM_BYTE> storeMaskAfterCall;
        std::vector<int> storeReadBitsAfterCall;
        VM_BYTE currentByte;
        int readBits;

        const std::list<Instruction> Instructions = {
            Instruction(VMINS_LOAD_CONST, VM_FIXED_LOAD_CONST, 3),
            // ..........
            Instruction(VMINS_CALL, VM_FIXED_CALL, 4),
        };

    public:
        VirtualMachineModel();
        ~VirtualMachineModel();

    private:
        VM_BYTE getBitFromCodeMemory();
        VM_BYTE convertLitleToBigEndian(VM_BYTE);
        VM_BYTE decodeArg();
        std::vector<VM_BYTE> decodeArgs(int);
        std::vector<VM_BYTE> decodeAddress();
        Instruction* decodeData();
        void printError(Instruction *instr);
};
#endif
