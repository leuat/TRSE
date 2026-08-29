#ifndef DIALOGNEW8BPLPROJECT_H
#define DIALOGNEW8BPLPROJECT_H

#include <QDialog>

namespace Ui {
class DialogNew8bplProject;
}

class DialogNew8bplProject : public QDialog
{
    Q_OBJECT

    QStringList m_values;
    QString m_8bplLoc, m_path;

public:
    explicit DialogNew8bplProject(QString plab, QString path, QWidget *parent = nullptr);
    ~DialogNew8bplProject();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DialogNew8bplProject *ui;
};

#endif // DIALOGNEW8BPLPROJECT_H
