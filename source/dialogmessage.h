#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

#include <QDialog>
#include <QString>
#include <QImage>

namespace Ui {
class DialogMessage;
}

class DialogMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMessage(QWidget *parent = 0);
    ~DialogMessage();

    void SetMessage(QString msg, QImage img);

    bool ignoreIsChecked();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogMessage *ui;
};

#endif // DIALOGMESSAGE_H
