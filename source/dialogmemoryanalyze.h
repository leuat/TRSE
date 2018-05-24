#ifndef DIALOGMEMORYANALYZE_H
#define DIALOGMEMORYANALYZE_H

#include <QDialog>
#include "source/Compiler/assembler/assembler.h"
#include <QMap>
#include <QImage>
#include <QPainter>
#include <QPixMap>


namespace Ui {
class DialogMemoryAnalyze;
}

class DialogMemoryAnalyze : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMemoryAnalyze(QWidget *parent = 0);
    void Initialize(QVector<MemoryBlock>& blocks);
    QMap<QString, QColor> m_colors;
    void InitColors();
    ~DialogMemoryAnalyze();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogMemoryAnalyze *ui;
};

#endif // DIALOGMEMORYANALYZE_H
