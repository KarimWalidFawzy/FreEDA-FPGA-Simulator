#ifndef WIRE_H
#define WIRE_H


class wire
{
private:
    float voltage;
    bool logic_state;

public:
    wire();
    wire(float voltage, bool logicState);

    void set_voltage(float value);
    float get_voltage() const;
    void set_logic_state(bool state);
    bool get_logic_state() const;
};

#endif // WIRE_H
