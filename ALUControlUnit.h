#include "ALU.h"
#include "LogicUnit.h"
#include <vector>
#include <array>

class ALUControlUnit : public Listener<6>, public Listener<2> {
    ALU* alu;
    Value<6> funcOP;
    Value<2> control;
    ALUop getOP() const;
  
  public:
    ALUControlUnit(ALU *alu);
    virtual void update_input(int num, Value<6> value) override;
    virtual void update_input(int num, Value<2> value) override;
};