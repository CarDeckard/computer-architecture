#include "LogicUnit.h"
class DirectAddress : public LogicUnit<32>, public Listener<26> {
    // private members here
    word FourSigBitMask;
    word TwentyEightLeastSigBitMask;

  public:
    virtual void update_input(int input, word value) override;
    virtual void update_input(int input, Value<26> value) override;
    virtual word get_output() const override;
};