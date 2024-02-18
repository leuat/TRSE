#ifndef DIALOGSELECTROOM_H
#define DIALOGSELECTROOM_H

#include <QDialog>
#include "source/LeLib/limage/imageleveleditor.h"
namespace Ui {
class DialogSelectRoom;
}

class DialogSelectRoom : public QDialog
{
    Q_OBJECT

public:
    QPoint m_level;
    explicit DialogSelectRoom(QWidget *parent = nullptr);
    ~DialogSelectRoom();
    void SetLevel(ImageLevelEditor* le);

private:
    Ui::DialogSelectRoom *ui;
};

#endif // DIALOGSELECTROOM_H
