#include "../include/core.hpp"

VM_BYTE VirtualMachineModel::getBitFromCodeMemory()
{
    VM_BYTE b;

    if(maskBitStream == 0b00000000)
    {
        maskBitStream = 0b10000000;
        registers->pc += 1;
    }
    if(maskBitStream == 0b10000000) 
    {
        currentByte = memory->codeArray[registers->pc];
        readBits = 1;
    }
    b = (currentByte & maskBitStream) >> (8 - readBits);
    maskBitStream >>= 1;
    ++readBits;
    return b;
}

VM_BYTE VirtualMachineModel::decodeArg()
{
    VM_BYTE b = 0;
    b |= getBitFromCodeMemory() | (getBitFromCodeMemory() << 1) 
                | (getBitFromCodeMemory() << 2) | (getBitFromCodeMemory() << 3);
    return b;
}

VM_BYTE VirtualMachineModel::convertLitleToBigEndian(VM_BYTE little)
{
    VM_BYTE big = 0;
    VM_BYTE mask = 0b10000000;
    for(int i = 1; i <= 8; i++)
    {
        VM_BYTE tmp = ((little & mask) >> (8 - i));
        if(i != 1) tmp <<= (i - 1);
        big |= tmp;
        mask >>= 1;
    }
    return big;
}

std::vector<VM_BYTE> VirtualMachineModel::decodeArgs(int howManyArg)
{
    std::vector<VM_BYTE> argData;
    VM_BYTE tmpByte = 0;
    for(int i = 0; i < howManyArg; i++)
    {
        VM_BYTE typeArg = getBitFromCodeMemory();
        argData.push_back(typeArg);
        if(typeArg == 0) argData.push_back(decodeArg());
        else if(typeArg == 1)
        {
            tmpByte = 0;
            tmpByte |= getBitFromCodeMemory() | (getBitFromCodeMemory() << 1);
            argData.push_back(tmpByte);
            argData.push_back(decodeArg());
        }
        else argData.clear();
    }
    return argData;
}


std::vector<VM_BYTE> VirtualMachineModel::decodeAddress()
{
    std::vector<VM_BYTE> address;
    VM_BYTE tmpByte = 0;
    int countBits = 0;
    while(countBits < 32)
    {
        ++countBits;
        tmpByte |= getBitFromCodeMemory();
        if((countBits % 8) != 0) tmpByte <<= 1;
        if((countBits % 8) == 0)
        {
            address.push_back(convertLitleToBigEndian(tmpByte));
            tmpByte = 0;
        }
    }
    return address;
}

void VirtualMachineModel::printError(Instruction *instr)
{
    std::cout << "ERROR: Unknown opcodes at " << std::dec << (registers->pc) << "!" << std::endl;
    delete instr;
    instr = NULL;
    isVMwork = false;
}

Instruction* VirtualMachineModel::decodeData()
{
    std::list<Instruction> copyInstructions(Instructions);
    Instruction *instr = NULL;
    
    VM_BYTE mask = 0b00000001;
    int countBit = 1;
    while(true)
    {
        VM_BYTE byte = getBitFromCodeMemory();
        std::list<Instruction> tmpCopyInstructions(copyInstructions);

        for(Instruction i : tmpCopyInstructions)
        {
            VM_BYTE opcode = ((i.getCode() & (mask << (i.getLength() - countBit))) >> ((i.getLength() - countBit)));
            if(opcode != byte) copyInstructions.remove(i);
        }
        ++countBit;
        if(copyInstructions.size() == 1)
        {
            for(Instruction i : copyInstructions) 
            {
                instr = new Instruction(i.getCode(), i.getFixedCode(), i.getLength());
                if(i.getLength() == countBit) getBitFromCodeMemory();
                std::cout << "[DEBUG]: Found opcode with fixed: " << std::dec << instr->getFixedCode() << std::endl;
            }
            break;
        }
        else if(copyInstructions.size() == 0)
        {
            std::cout << "ERROR: Unknown opcodes at " << std::dec << (registers->pc) << "!" << std::endl;
            isVMwork = false;
            return NULL;
        }
    }

    switch(instr->getFixedCode())
    {
        case VM_FIXED_LOAD_CONST:
            {
                std::vector<VM_BYTE> argData;
                VM_BYTE tmpByte = 0;
                int countBits = 0;
                while(countBits < 64)
                {
                    tmpByte |= getBitFromCodeMemory();
                    ++countBits;
                    if((countBits % 8) != 0) tmpByte <<= 1;
                    if((countBits % 8) == 0)
                    {
                        argData.push_back(convertLitleToBigEndian(tmpByte));
                        tmpByte = 0;
                    }
                }
                std::vector<VM_BYTE> tmp = decodeArgs(1);
                if(tmp.size() == 0) printError(instr);
                argData.insert(argData.end(), tmp.begin(), tmp.end());
                instr->setArgsData(argData);
            }
            break;
        // ..........
        case VM_FIXED_CALL:
            instr->setArgsData(decodeAddress());
            break;
        default:
            printError(instr);
    }
    return instr;
}
