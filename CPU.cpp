#include "CPU.h"

#include <iostream>
#include <climits>

CPU::CPU(Clock* clock, Memory* imem, Memory* dmem)
    : controlUnit(clock), registerFile(clock), PC(clock, 0x1000), opCode(26, 31),
      rs(21, 25), rt(16, 20), rd(11, 15), shamt(6, 10), func(0, 5),
      immediateValue(0, 15), jumpAddress(0, 25), aluControlUnit(&mainALU){

    controlUnit.register_listener(ControlUnit::InstRequest, imem, 0);
    controlUnit.register_listener(ControlUnit::InstReadWrite, imem, 1);
    imem->register_wait_listener(&controlUnit, ControlUnit::InstWait);

    controlUnit.register_listener(ControlUnit::DataRequest, dmem, 0);
    controlUnit.register_listener(ControlUnit::DataReadWrite, dmem, 1);
    dmem->register_wait_listener(&controlUnit, ControlUnit::DataWait);

    controlUnit.reset();

    plus4ALU.update_input(1, word{4});
    plus4ALU.update_input(0, toValue(ALUop::Sum));

    signExtender.register_listener(&leftShifter, 0);

    plus4ALU.Reporter<32>::register_listener(&plusBranchALU, 0);
    leftShifter.register_listener(&plusBranchALU, 1);
    plusBranchALU.update_input(0, toValue(ALUop::Sum));

    controlUnit.register_listener(ControlUnit::Branch, &branchOption, 0);
    mainALU.MultiReporter<1, num_codes>::register_listener(ALU::Zero, &branchOption, 1);

    plus4ALU.Reporter<32>::register_listener(&BranchMux, 0);
    plusBranchALU.Reporter<32>::register_listener(&BranchMux, 1);
    branchOption.register_listener(&BranchMux, 2);

    BranchMux.register_listener(&PCMux, 0);
    directAddress.register_listener(&PCMux, 1);
    registerFile.register_listener(0, &PCMux, 2);
    controlUnit.register_listener(ControlUnit::PCSrc, &PCMux, 4);

    PCMux.register_listener(&PC, 0);
    controlUnit.register_listener(ControlUnit::PCEnable, &PC, 0);

    PC.register_listener(&plus4ALU, 0);
    PC.register_listener(imem, 0);

    imem->register_listener(&opCode, 0);
    imem->register_listener(&rs, 0);
    imem->register_listener(&rt, 0);
    imem->register_listener(&rd, 0);
    imem->register_listener(&shamt, 0);
    imem->register_listener(&func, 0);
    imem->register_listener(&immediateValue, 0);
    imem->register_listener(&jumpAddress, 0);    

    PC.register_listener(&directAddress, 0);
    jumpAddress.register_listener(&directAddress, 1);

    registerFile.register_listener(0, &mainALU, 0);
    ALUsrc2Mux.register_listener(&mainALU, 1);

    rs.register_listener(&registerFile, 0);
    rt.register_listener(&registerFile, 1);
    WriteEnableMux.register_listener(&registerFile, 2);
    WriteDataMux.register_listener(&registerFile, 3);
    controlUnit.register_listener(ControlUnit::RegWrite, &registerFile, 4);

    rt.register_listener(&WriteEnableMux, 0);
    rd.register_listener(&WriteEnableMux, 1);
    WriteEnableMux.update_input(2, Value<5>{31});
    controlUnit.register_listener(ControlUnit::WriteRegDst, &WriteEnableMux, 2);    

    shamt.register_listener(&shamtExtender, 0);

    immediateValue.register_listener(&signExtender, 0);
    controlUnit.register_listener(ControlUnit::SignExtend, &signExtender, 0);

    registerFile.register_listener(1, &ALUsrc2Mux, 0);
    signExtender.register_listener(&ALUsrc2Mux, 1);
    shamtExtender.register_listener(&ALUsrc2Mux, 2);
    plus4ALU.Reporter<32>::register_listener(&ALUsrc2Mux, 3);
    controlUnit.register_listener(ControlUnit::ALUSrc2, &ALUsrc2Mux, 2);

    mainALU.Reporter<32>::register_listener(dmem, 0);
    registerFile.register_listener(1, dmem, 1);

    mainALU.Reporter<32>::register_listener(&WriteDataMux, 0);
    dmem->register_listener(&WriteDataMux, 1);
    plus4ALU.Reporter<32>::register_listener(&WriteDataMux, 2);
    controlUnit.register_listener(ControlUnit::MemToReg, &WriteDataMux, 4);

    controlUnit.register_listener(ControlUnit::ALUOpType, &aluControlUnit, 0);

    opCode.register_listener(&controlUnit, 0);
    func.register_listener(&controlUnit, 1);
    func.register_listener(&aluControlUnit, 0);
}