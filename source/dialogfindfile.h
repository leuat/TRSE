#ifndef DIALOGFINDFILE_H
#define DIALOGFINDFILE_H

#include <QDialog>

namespace Ui {
class DialogFindFile;
}

class DialogFindFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFindFile(QWidget *parent = nullptr);
    ~DialogFindFile();

private:
    Ui::DialogFindFile *ui;
};

#endif // DIALOGFINDFILE_H
