#include "gates.h"

gate::gate()
    : gate(Type::And)
{
}

gate::gate(Type gateType)
{
    set_type(gateType);
}

gate::gate(wire firstInput, wire secondInput, Type gateType)
    : input_1(firstInput), input_2(secondInput)
{
    set_type(gateType);
}

void gate::set_type(Type gateType)
{
    switch (gateType)
    {
    case Type::And:
        logical_function = [](bool a, bool b) { return a && b; };
        break;
    case Type::Or:
        logical_function = [](bool a, bool b) { return a || b; };
        break;
    case Type::Not:
        logical_function = [](bool a, bool) { return !a; };
        break;
    case Type::Nand:
        logical_function = [](bool a, bool b) { return !(a && b); };
        break;
    case Type::Nor:
        logical_function = [](bool a, bool b) { return !(a || b); };
        break;
    case Type::Xor:
        logical_function = [](bool a, bool b) { return a != b; };
        break;
    case Type::Xnor:
        logical_function = [](bool a, bool b) { return a == b; };
        break;
    case Type::Buffer:
        logical_function = [](bool a, bool) { return a; };
        break;
    }
}

void gate::set_inputs(wire firstInput, wire secondInput)
{
    input_1 = firstInput;
    input_2 = secondInput;
}

wire gate::get_output() const
{
    return output;
}

bool gate::evaluate()
{
    const bool result = logical_function(
        input_1.get_logic_state(),
        input_2.get_logic_state());
    output.set_logic_state(result);
    return result;
}
