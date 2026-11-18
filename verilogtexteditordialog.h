#ifndef VERILOGTEXTEDITORDIALOG_H
#define VERILOGTEXTEDITORDIALOG_H

#include <QDialog>

namespace Ui {
class VerilogTextEditorDialog;
}

class VerilogTextEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VerilogTextEditorDialog(QWidget *parent = nullptr);
    ~VerilogTextEditorDialog();

private:
    Ui::VerilogTextEditorDialog *ui;
};

#endif // VERILOGTEXTEDITORDIALOG_H
