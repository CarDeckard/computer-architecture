#include "ALUControlUnit.h"

ALUControlUnit::ALUControlUnit(ALU *alu) : alu(alu), control(0), funcOP(0){}

ALUop ALUControlUnit::getOP() const{
    switch (funcOP){
        case 0x00:
            return ALUop::LeftShift;
        case 0x02:
            return ALUop::RightShift;
        case 0x22:
            return ALUop::Sub;
        case 0x26:
            return ALUop::Xor;
        case 0x27:
            return ALUop::Nor;
        case 0x2A:
            return ALUop::SetLessThan;
        default:
            return ALUop::Sum;
    }
}

void ALUControlUnit::update_input(int num, Value<6> value) {
    funcOP = value;
    if(control == 2){
        alu->update_input(0, toValue( getOP() ));
    }
}


void ALUControlUnit::update_input(int num, Value<2> value) {
    control = value;
    if (value == 0){
        alu->update_input(0, toValue(ALUop::Sum));
    }else if (value == 1){
        alu->update_input(0, toValue(ALUop::Sub));
    }else if (value == 2){
        alu->update_input(0, toValue(getOP()));
    }else if (value == 3){
        alu->update_input(0, toValue(ALUop::Or));
    }

}