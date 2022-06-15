#include "LogicUnit.h"
template<int in, int out>
class Extender : public Listener<in>, public Reporter<out>, public Listener<1> {
    // private members here
    Value<1> SignBit;
    Value<in> LeastSigBits;

  public:
    virtual void update_input(int input, Value<in> value) override;
    virtual void update_input(int input, Value<1> value) override;
    virtual Value<out> get_output() const override;
};

template<int in, int out>
void Extender<in, out>::update_input(int input, Value<in> value){
    LeastSigBits = value;
    this->update_listeners();
}

template<int in, int out>
void Extender<in, out>::update_input(int input, Value<1> value){
    SignBit = value;
    this->update_listeners();
}

template<int in, int out>
Value<out> Extender<in, out>::get_output() const{
    int MSB =  SignBit & LeastSigBits >> (in - 1);
    Value<out> temp;
    if(MSB){
        temp = (1 << (out- in)) - 1;
        temp = temp << in;
        return temp | LeastSigBits;
    }else{
        return LeastSigBits & -1;
    }
}