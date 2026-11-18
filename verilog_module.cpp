#include "verilog_module.h"
#include <utility>

verilog_module::verilog_module() = default;

verilog_module::verilog_module(QString moduleName)
    : name(std::move(moduleName))
{
}

void verilog_module::set_name(const QString &moduleName)
{
    name = moduleName;
}

QString verilog_module::get_name() const
{
    return name;
}

void verilog_module::add_input(const QString &portName)
{
    if (!portName.isEmpty() && !inputs.contains(portName))
        inputs.append(portName);
}

void verilog_module::add_output(const QString &portName)
{
    if (!portName.isEmpty() && !outputs.contains(portName))
        outputs.append(portName);
}

void verilog_module::add_inout(const QString &portName)
{
    if (!portName.isEmpty() && !inouts.contains(portName))
        inouts.append(portName);
}

void verilog_module::add_wire(const QString &wireName)
{
    if (!wireName.isEmpty() && !wires.contains(wireName))
        wires.append(wireName);
}

void verilog_module::add_reg(const QString &registerName)
{
    if (!registerName.isEmpty() && !regs.contains(registerName))
        regs.append(registerName);
}

const QVector<QString> &verilog_module::get_inputs() const { return inputs; }
const QVector<QString> &verilog_module::get_outputs() const { return outputs; }
const QVector<QString> &verilog_module::get_inouts() const { return inouts; }
const QVector<QString> &verilog_module::get_wires() const { return wires; }
const QVector<QString> &verilog_module::get_regs() const { return regs; }
