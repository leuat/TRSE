#ifndef DIALOGMEMORYANALYZE_H
#define DIALOGMEMORYANALYZE_H

#include <QDialog>
#include "source/Compiler/assembler/assembler.h"
#include <QMap>
#include <QImage>
#include <QPainter>
#include <QPixmap>


namespace Ui {
class DialogMemoryAnalyze;
}

class DialogMemoryAnalyze : public QDialog
{
    Q_OBJECT

    int m_fontSize;
    QVector<MemoryBlock*> m_blocks;
public:
    explicit DialogMemoryAnalyze(QWidget *parent = 0);
    void Initialize(QVector<MemoryBlock*>& blocks, int fontSize);
    QMap<QString, QColor> m_colors;
    void InitColors();
    void resizeEvent(QResizeEvent *) override;
    void VerifyZPMusic(QVector<MemoryBlock*> &blocks);
    ~DialogMemoryAnalyze();

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogMemoryAnalyze *ui;
};

#endif // DIALOGMEMORYANALYZE_H
