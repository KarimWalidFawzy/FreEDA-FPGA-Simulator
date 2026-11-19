#include "gatesdiagram.h"
#include "ui_gatesdiagram.h"

GatesDiagram::GatesDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GatesDiagram)
{
    ui->setupUi(this);
}

GatesDiagram::~GatesDiagram()
{
    delete ui;
}
