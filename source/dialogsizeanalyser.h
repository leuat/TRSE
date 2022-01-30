#ifndef DIALOGSIZEANALYSER_H
#define DIALOGSIZEANALYSER_H

#include <QDialog>
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/sourcebuilder.h"

namespace Ui {
class DialogSizeAnalyser;
}

class SizeData {
public:
    QString name;
    float val;
    int org, type;
    SizeData(QString n, int v, int t) {
        name = n;
        val = v;
        org = v;
        type = t;
    }
    SizeData() {}

};


class DialogSizeAnalyser : public QDialog
{
    Q_OBJECT

    bool m_success;
    int m_fontSize = 10;
    int xsize, ysize,shift,time;
    CIniFile* m_ini = nullptr;
    QString curT, prevT;
    QPointF zoomCenter,cur;
    float zoomVal;

    float maxVal;
    QPoint mpos;
    SourceBuilder* m_sb = nullptr;
    QVector<SizeData> m_data;

public:
    explicit DialogSizeAnalyser(QWidget *parent = nullptr);
    ~DialogSizeAnalyser();


    void Initialize(SourceBuilder* sb, CIniFile* ini, int fontSize);
    void Build();
    void ReInitialise();
    QMap<QString, QColor> m_colors;
    void InitColors();
    void resizeEvent(QResizeEvent *) override;

    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

    QPointF Trans(QPointF p);
    QRect Trans(QRect r, float addy = 0);
    QRect Trans(float, float, float, float);



private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogSizeAnalyser *ui;
};

#endif // DIALOGSIZEANALYSER_H
