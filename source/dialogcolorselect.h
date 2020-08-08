#ifndef DIALOGCOLORSELECT_H
#define DIALOGCOLORSELECT_H

#include <QDialog>
#include <QVector3D>

namespace Ui {
class DialogColorSelect;
}

class DialogColorSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogColorSelect(QWidget *parent = nullptr);
    ~DialogColorSelect();
    bool m_ok = true;
    QVector3D m_bpp;
    QColor m_color;
    void Init(QColor color, QVector3D bpp);

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

private:
    Ui::DialogColorSelect *ui;
};

#endif // DIALOGCOLORSELECT_H
