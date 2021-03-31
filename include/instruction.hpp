#ifndef _INSTRUCTION_HPP
#define _INSTRUCTION_HPP

#include "global.hpp"

#include <iostream>
#include <vector>

class Instruction {
    private:
        VM_BYTE code;
        int fixedCode;
        short length;
        short argsCount;
        std::vector<VM_BYTE> argsData;
        
// Methods
    public:
        Instruction(VM_BYTE code, VM_BYTE fixedCode, short length)
        {
            this->code = code;
            this->fixedCode = fixedCode;
            this->length = length;
        }

        Instruction(VM_BYTE code, VM_BYTE fixedCode, short length, short argsCount, std::vector<VM_BYTE> argsData)
        {
            this->code = code;
            this->fixedCode = fixedCode;
            this->length = length;
            this->argsCount = argsCount;
            this->argsData = argsData;
        }

        bool operator == (const Instruction& i) const { return fixedCode == i.fixedCode; }
        bool operator != (const Instruction& i) const { return !operator==(i); }

        VM_BYTE getCode() { return code; }
        int getFixedCode() { return fixedCode; }
        short getLength() { return length; }
        short getArgsCount() { return argsCount; }
        std::vector<VM_BYTE> getArgsData() { return argsData; }

        void setCode(VM_BYTE v) { code = v; }
        void setFixedCode(int v) { fixedCode = v; }
        void setLength(short v) { length = v; }
        void setArgsCount(short v) { argsCount = v; }
        void setArgsData(std::vector<VM_BYTE> v) { argsData = v; }
};

#endif
