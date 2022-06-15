#include "ALU.h"

Value<3> toValue(ALUop op) {
    return {static_cast<unsigned long>(op)};
}

ALU::ALU() {
    // implement ALU constructor here
    ALUinputs[0] = 0;
    ALUinputs[1] = 0;
    ALUoutput = 0;

    ALUflag[0] = 0;
    ALUflag[1] = 0;
    ALUflag[2] = 0;
    ALUflag[3] = 0;
}

Value<1> ALU::get_output(int output, Value<1> ignore) const {
    // return condition code associated with output
    return ALUflag[output];
}

word ALU::get_output() const {
    // return computed value from ALU
    return ALUoutput;
}

void ALU::update_input(int num, Value<3> value) {
    // receive new operation
    ALUopCode = value;
    ALUoperation(ALUopCode);
    Reporter::update_listeners();
    MultiReporter::update_listeners();
}

void ALU::update_input(int num, word value) {
    // receive new input value
    ALUinputs[num] = value;
    ALUoperation(ALUopCode);
    Reporter::update_listeners();
    MultiReporter::update_listeners();
}

void ALU::ALUoperation(int OPcode) {
    switch (OPcode){
        //Left Shift
        case 0:
            ALUoutput = ALUinputs[0] << ALUinputs[1];
            break;

        //Nor
        case 1:
            ALUoutput = ~ (ALUinputs[0] | ALUinputs[1]);
            break;

        //Or
        case 2:
            ALUoutput = ALUinputs[0] | ALUinputs[1];
            break;

        //Right Shift
        case 3:
            ALUoutput = ALUinputs[0] >> ALUinputs[1];
            break;

        //Set less than
        case 4:
            if(ALUinputs[0] < ALUinputs[1]){
                ALUoutput = 1;
            }else{
                ALUoutput = 0; 
            }
            break;

        //Sub
        case 5:
            ALUoutput = ALUinputs[0] - ALUinputs[1];
            break;

        //Sum
        case 6:
            ALUoutput = ALUinputs[0] + ALUinputs[1];
            break;

        //Xor
        case 7:
            ALUoutput = ALUinputs[0] ^ ALUinputs[1];
            break;
    }
    setConditionCodes();
}
void ALU::setZeroFlag() {
    if(ALUoutput == 0){
        ALUflag[0] = 1;
    }else{
        ALUflag[0] = 0;
    }
}

void ALU::setNegativeFlag() {
    if(ALUoutput < 0){
        ALUflag[1] = 1;
    }else{
        ALUflag[1] = 0;
    }
}

void ALU::setOverFlowFlag() {
    if( ( ((ALUinputs[0] && ALUinputs[1]) > 0) && (ALUoutput < 0) ) || ( ( (ALUinputs[0] && ALUinputs[1]) < 0) && (ALUoutput > 0) ) ){
        ALUflag[2] = 1; 
    }else{
        ALUflag[2] = 0;
    }
}

void ALU::setCarryOverFlag() {}

void ALU::setConditionCodes() {

    setZeroFlag();
    setNegativeFlag();
    setOverFlowFlag();
    setCarryOverFlag();

}

