#ifndef GATESDIAGRAM_H
#define GATESDIAGRAM_H

#include <QDialog>

namespace Ui {
class GatesDiagram;
}

class GatesDiagram : public QDialog
{
    Q_OBJECT

public:
    explicit GatesDiagram(QWidget *parent = nullptr);
    ~GatesDiagram();

private:
    Ui::GatesDiagram *ui;
};

#endif // GATESDIAGRAM_H
