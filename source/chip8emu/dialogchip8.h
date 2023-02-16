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
    QSharedPointer<QTimer> timer = nullptr;
    explicit dialogchip8(QString file,QWidget *parent = nullptr);
    ~dialogchip8();

    void stop();

private slots:
    void on_pushButton_clicked();

private:
    Ui::dialogchip8 *ui;
};

#endif // DIALOGCHIP8_H
