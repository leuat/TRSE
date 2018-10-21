#ifndef DIALOGFINDFILE_H
#define DIALOGFINDFILE_H

#include <QDialog>
#include <QKeyEvent>
#include <QListWidgetItem>
namespace Ui {
class DialogFindFile;
}

class DialogFindFile : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFindFile(QWidget *parent = nullptr);
    ~DialogFindFile();
    void Init(QVector<QString> files);
    QVector<QString> m_shortFile;
    QVector<QString> m_fullFile;
    QString m_selected;
    void keyPressEvent(QKeyEvent* e);
    int cur=0;

    void Update();

private slots:
    void on_leSearch_textChanged(const QString &arg1);

    void on_leSearch_returnPressed();

    void on_lstItems_itemDoubleClicked(QListWidgetItem *item);

    void on_lstItems_itemPressed(QListWidgetItem *item);

private:
    Ui::DialogFindFile *ui;
};

#endif // DIALOGFINDFILE_H
