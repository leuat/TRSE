#ifndef DIALOGSPLASH_H
#define DIALOGSPLASH_H

#include <QDialog>

namespace Ui {
class DialogSplash;
}

class DialogSplash : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSplash(QWidget *parent = nullptr);
    ~DialogSplash();

    void Init();

private:
    Ui::DialogSplash *ui;


public slots:
    void CloseMe();

};

#endif // DIALOGSPLASH_H
