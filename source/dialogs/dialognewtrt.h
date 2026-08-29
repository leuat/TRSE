#ifndef DIALOGNEWTRT_H
#define DIALOGNEWTRT_H

#include <QDialog>

namespace Ui {
class DialogNewTRT;
}

class DialogNewTRT : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewTRT(QWidget *parent = nullptr);
    ~DialogNewTRT();
    bool cancel = false;
    int getRows();
    int getChannels();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();


private:
    Ui::DialogNewTRT *ui;
};

#endif // DIALOGNEWTRT_H
