#ifndef VERILOG_MODULE_H
#define VERILOG_MODULE_H

#include <QString>
#include <QVector>

class verilog_module
{
private:
    QString name;
    QVector<QString> inputs;
    QVector<QString> outputs;
    QVector<QString> inouts;
    QVector<QString> wires;
    QVector<QString> regs;

public:
    verilog_module();
    explicit verilog_module(QString moduleName);

    void set_name(const QString &moduleName);
    QString get_name() const;

    void add_input(const QString &name);
    void add_output(const QString &name);
    void add_inout(const QString &name);
    void add_wire(const QString &name);
    void add_reg(const QString &name);

    const QVector<QString> &get_inputs() const;
    const QVector<QString> &get_outputs() const;
    const QVector<QString> &get_inouts() const;
    const QVector<QString> &get_wires() const;
    const QVector<QString> &get_regs() const;
};

#endif // VERILOG_MODULE_H
