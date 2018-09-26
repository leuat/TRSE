#ifndef DIALOGIMAGEHELP_H
#define DIALOGIMAGEHELP_H

#include <QDialog>

namespace Ui {
class DialogImageHelp;
}

class DialogImageHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImageHelp(QWidget *parent = nullptr);
    ~DialogImageHelp();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogImageHelp *ui;
};

#endif // DIALOGIMAGEHELP_H
