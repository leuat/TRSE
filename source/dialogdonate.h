#ifndef DIALOGDONATE_H
#define DIALOGDONATE_H

#include <QDialog>

namespace Ui {
class DialogDonate;
}

class DialogDonate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDonate(QWidget *parent = nullptr);
    ~DialogDonate();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogDonate *ui;
};

#endif // DIALOGDONATE_H
