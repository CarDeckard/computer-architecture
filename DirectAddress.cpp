#include "DirectAddress.h"

void DirectAddress::update_input(int input, word value){
    this->FourSigBitMask = (value >> 28) << 28;
    this->update_listeners();
}

void DirectAddress::update_input(int input, Value<26> value){
    this->TwentyEightLeastSigBitMask = value << 2;
    this->update_listeners();
}

word DirectAddress::get_output() const{
    return this->FourSigBitMask | this->TwentyEightLeastSigBitMask;
}