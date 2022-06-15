#include "LogicUnit.h"
template<int N>
class AndGate : public LogicUnit<N> {
    // private members here
    Value<N> Input1;
    Value<N> Input2;

  public:
    virtual void update_input(int num, Value<N> value) override;
    virtual Value<N> get_output() const override;
};

template<int N>
void AndGate<N>::update_input(int num, Value<N> value){
    if(num  == 0){
        this->Input1 = value;
    }else if (num == 1){
        this->Input2 = value;
    }
    this->update_listeners();
    return;
}

template<int N>
Value<N> AndGate<N>::get_output() const{
    return this->Input1 & this->Input2;
}