#ifndef CPU_H
#define CPU_H

#include "ALU.h"
#include "Clock.h"
#include "common.h"
#include "DirectAddress.h"
#include "Event.h"
#include "Extender.h"
#include "Gates.h"
#include "Mux.h"
#include "RegisterFile.h"
#include "Register.h"
#include "Shifter.h"
#include "ValueSlice.h"
#include "ControlUnit.h"
#include "ALUControlUnit.h"
#include "Memory.h"

#include <queue>

class CPU {
    ControlUnit controlUnit;
    ALUControlUnit aluControlUnit;
    RegisterFile registerFile;
    DirectAddress directAddress;

    Mux<4, 32> PCMux;
    Mux<4, 32> WriteDataMux;
    Mux<4, 32> ALUsrc2Mux;
    Mux<2, 32> BranchMux;
    Mux<4, 5> WriteEnableMux;

    ValueSlice<32, 5> rt; 
    ValueSlice<32, 5> rd; 
    ValueSlice<32, 5> rs;
    ValueSlice<32, 5> shamt;
    ValueSlice<32, 6> opCode;
    ValueSlice<32, 6> func;
    ValueSlice<32, 16> immediateValue; 
    ValueSlice<32, 26> jumpAddress;

    ALU mainALU;
    ALU plus4ALU;
    ALU plusBranchALU;
    
    Extender<5, 32> shamtExtender;
    Extender<16, 32> signExtender;

    Register<32> PC;

    LeftShifter<32, 2> leftShifter;

    AndGate<1> branchOption;

    public:
        CPU(Clock* clock, Memory* imem, Memory* dmem);
};

#endif