#include "verilogtexteditordialog.h"
#include "ui_verilogtexteditordialog.h"

VerilogTextEditorDialog::VerilogTextEditorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerilogTextEditorDialog)
{
    ui->setupUi(this);
}

VerilogTextEditorDialog::~VerilogTextEditorDialog()
{
    delete ui;
}
