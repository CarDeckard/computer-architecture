#ifndef ALU_H
#define ALU_H

#include <vector>
#include <array>

#include "LogicUnit.h"
#include "common.h"

enum class ALUop {
    LeftShift,
    Nor,
    Or,
    RightShift,
    SetLessThan,
    Sub,
    Sum,
    Xor,
};

Value<3> toValue(ALUop op);

const static int num_codes = 4;
class ALU : public LogicUnit<32>, public Listener<3>,
	    public MultiReporter<1 , num_codes>
{
    // member data and functions here
    word ALUinputs[2];
    int ALUflag[4];
    word ALUoutput;
    int ALUopCode;
    void ALUoperation(int OPcode);
    void setConditionCodes();
    void setZeroFlag();
    void setNegativeFlag();
    void setOverFlowFlag();
    void setCarryOverFlag();

  public:
    ALU();

    virtual void update_input(int num, word value) override;
    virtual void update_input(int num, Value<3> value) override;
    virtual word get_output() const override;
    virtual Value<1> get_output(int output, Value<1> ignore) const override;

    const static int Zero = 0;
    const static int Negative = 1;
    const static int Overflow = 2;
    const static int CarryOut = 3;
};

#endif
