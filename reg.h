#ifndef REG_H
#define REG_H

#include "wire.h"
class reg
{
    private:
    bool logic_state;
    wire clk;
    bool latch_flip_flop;    
public:
    reg();
};

#endif // REG_H
