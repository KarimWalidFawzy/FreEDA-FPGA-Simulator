#include "wire.h"

wire::wire()
    : voltage(0.0f), logic_state(false)
{
}

wire::wire(float initialVoltage, bool initialLogicState)
    : voltage(initialVoltage), logic_state(initialLogicState)
{
}

void wire::set_voltage(float value)
{
    voltage = value;
}

float wire::get_voltage() const
{
    return voltage;
}

void wire::set_logic_state(bool state)
{
    logic_state = state;
}

bool wire::get_logic_state() const
{
    return logic_state;
}
