#ifndef GATES_H
#define GATES_H

#include "wire.h"
#include <functional>

class gate
{
public:
    enum class Type
    {
        And,
        Or,
        Not,
        Nand,
        Nor,
        Xor,
        Xnor,
        Buffer
    };

private:
    wire input_1;
    wire input_2;
    wire output;
    std::function <bool(bool,bool)> logical_function;

public:
    gate();
    explicit gate(Type gateType);
    gate(wire firstInput, wire secondInput, Type gateType);

    void set_type(Type gateType);
    void set_inputs(wire firstInput, wire secondInput);
    wire get_output() const;
    bool evaluate();
};

#endif // GATES_H
