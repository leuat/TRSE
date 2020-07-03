#ifndef DIALOGSIMPLELINEEDIT_H
#define DIALOGSIMPLELINEEDIT_H

#include <QDialog>

namespace Ui {
class DialogSimpleLineEdit;
}

class DialogSimpleLineEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSimpleLineEdit(QString wt, QString info, QString val, QWidget *parent = nullptr);
    ~DialogSimpleLineEdit();
    bool m_ok = true;
    QString getValue();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogSimpleLineEdit *ui;
};

#endif // DIALOGSIMPLELINEEDIT_H
