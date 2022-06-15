#include "LogicUnit.h"
template<int N, int size>
class LeftShifter : public LogicUnit<N> {
    // private members here
    Value<N> ShiftOutput;

  public:
    virtual void update_input(int input, Value<N> value) override;
    virtual Value<N> get_output() const override;
};

template<int N, int size>
void LeftShifter<N, size>::update_input(int input, Value<N> value){
    if( input == 0 ){
        this->ShiftOutput = Value<N>(value << size);
    }
    this->update_listeners();
    return;
}

template<int N, int size>
Value<N> LeftShifter<N, size>::get_output() const{
    return this->ShiftOutput;
}
