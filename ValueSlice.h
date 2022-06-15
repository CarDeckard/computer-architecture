#ifndef VALUE_SLICE_H
#define VALUE_SLICE_H

#include <string> 
#include "LogicUnit.h"

template<int in, int out>
class ValueSlice : public Listener<in>, public Reporter<out> {
    // private members here
    Value<out> output;
    int start;
    int stop;

    std::string prefix;

  public:
    ValueSlice(int start, int stop, const std::string& prefix = "");

    virtual void update_input(int input, Value<in> value) override;
    virtual Value<out> get_output() const override;
};

template<int in, int out>
ValueSlice<in, out>::ValueSlice(int first, int last, const std::string& pre)
    : start(first), stop(last), output(0), prefix(pre){}

template<int in, int out>
void ValueSlice<in, out>::update_input(int input, Value<in> value) {
    output = (value >> start) & ((1 << (stop - start + 1 )) - 1); 
    this->update_listeners();
}

template<int in, int out>
Value<out> ValueSlice<in, out>::get_output() const {
    return output;
}

#endif