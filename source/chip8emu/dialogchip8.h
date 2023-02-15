#ifndef DIALOGCHIP8_H
#define DIALOGCHIP8_H

#include <QDialog>
#include "chip8emu.h"

namespace Ui {
class dialogchip8;
}

class dialogchip8 : public QDialog
{
    Q_OBJECT



public:
    Chip8Emu c8;
    QTimer* timer = nullptr;
    explicit dialogchip8(QWidget *parent = nullptr);
    ~dialogchip8();

private slots:
    void on_pushButton_clicked();

private:
    Ui::dialogchip8 *ui;
};

#endif // DIALOGCHIP8_H
