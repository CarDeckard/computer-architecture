#include "RegisterFile.h"

RegisterFile::RegisterFile(Clock* clock) {
    for( int i = 0; i < 32; i++){
        RegisterArray.push_back(Register<32>(clock));
        RegisterArray[i].register_listener(this, i+5);
    }
}

void RegisterFile::update_input(int num, word value) {
    if (num == 3){
        RegisterArray[DestinationRegister].update_input(0, value);
        RegisterNextValue = value;
    }else{
        update_listeners();
    }
}

void RegisterFile::update_input(int num, Value<5> value) {
    switch(num){
        case 0: 
            RegisterOutput1 = value;
            update_listeners();
            break;
        case 1:
            RegisterOutput2 = value;
            update_listeners();
            break;
        case 2:
            RegisterArray[DestinationRegister].update_input(0, Value<1>{0});
            DestinationRegister = value;
            if(DestinationRegister != Value<5>{0}){
                RegisterArray[DestinationRegister].update_input(0, Value<1>{1});
            } else {
                update_listeners();
            }
            RegisterArray[DestinationRegister].update_input(0, RegisterNextValue);
            break;
    }
}

void RegisterFile::update_input(int num, Value<1> value) {
    if( num == 4 ){
        RegisterWriteEnable = value;
        if( DestinationRegister != Value<5>{0}){
            RegisterArray[DestinationRegister].update_input(0, RegisterWriteEnable);
        }
    }
}

word RegisterFile::get_output(int output, word ignore) const {
    if (output == 0){
        return RegisterArray[RegisterOutput1].get_output();
    }else if (output == 1){
        return RegisterArray[RegisterOutput2].get_output();
    }else{
        return 0;
    }
}