#include "voltagetimingdiagram.h"
#include "ui_voltagetimingdiagram.h"

VoltageTimingDiagram::VoltageTimingDiagram(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VoltageTimingDiagram)
{
    ui->setupUi(this);
}

VoltageTimingDiagram::~VoltageTimingDiagram()
{
    delete ui;
}
