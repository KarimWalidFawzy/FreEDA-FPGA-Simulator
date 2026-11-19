#ifndef VOLTAGETIMINGDIAGRAM_H
#define VOLTAGETIMINGDIAGRAM_H

#include <QDialog>

namespace Ui {
class VoltageTimingDiagram;
}

class VoltageTimingDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit VoltageTimingDiagram(QWidget *parent = nullptr);
    ~VoltageTimingDiagram();

private:
    Ui::VoltageTimingDiagram *ui;
};

#endif // VOLTAGETIMINGDIAGRAM_H
