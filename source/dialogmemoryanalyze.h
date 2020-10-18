/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DIALOGMEMORYANALYZE_H
#define DIALOGMEMORYANALYZE_H

#include <QDialog>
#include "source/Compiler/assembler/assembler.h"
#include <QMap>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include "source/Compiler/systems/abstractsystem.h"
#include <QToolTip>
#include <QWheelEvent>
namespace Ui {
class DialogMemoryAnalyze;
}

class DialogMemoryAnalyze : public QDialog
{
    Q_OBJECT

    int m_fontSize,xsize,ysize,shift;
    QVector<QSharedPointer<MemoryBlock>> m_blocks;
    QSharedPointer<CIniFile> m_iniFile = nullptr;
    AbstractSystem *m_system = nullptr;
    QString curT = "";
    QPoint cur;
    QPoint mpos;
    float delta;

    QPoint zoomCenter;
    float zoomVal = 1;
    int time = 0;

public:
    bool m_success;
    explicit DialogMemoryAnalyze(QSharedPointer<CIniFile> ini, AbstractSystem* system, QWidget *parent = 0);
    void Initialize(QVector<QSharedPointer<MemoryBlock>>& blocks, int fontSize);
    int m_noBanks = 0;
    QMap<QString, QColor> m_colors;
    void InitColors();
    void RenderSystemLabels(QPainter& p,int,int);
    void resizeEvent(QResizeEvent *) override;
    void VerifyZPMusic(QVector<QSharedPointer<MemoryBlock>> &blocks);
    ~DialogMemoryAnalyze();

    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent* event) override;

    QPointF Trans(QPointF p);
    QRect Trans(QRect r, float addy = 0);
    QRect Trans(float, float, float, float);

private slots:
    void on_btnClose_clicked();

private:
    Ui::DialogMemoryAnalyze *ui;
};

#endif // DIALOGMEMORYANALYZE_H
