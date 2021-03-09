/*
 *
 * Hexview is originally based on https://github.com/virinext/QHexView
 * by Victor Anjin. Extended by Leuat.
 *
 *
 *
 * */

#ifndef Q_TTR_VIEWER_H_
#define Q_TTR_VIEWER_H_

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QFile>
#include "source/trsedocuments/trsedocument.h"
#include "source/LeLib/util/cinifile.h"
#include "source/ImageEditor/hexview.h"
#include <QSharedPointer>
class TTRView: public QAbstractScrollArea

{
    public:
        TRSEDocument* m_doc = nullptr;
        QStringList notes = QStringList() << "C "<<"C#"<<"D "<<"D#"<<"E "<<"F "<<"F#"<<"G "<<"G#"<<"A "<<"A#"<<"B ";


        QString validHex = "abcdef0123456789-";
        QString validNotes1 = "cdefgab-";
        QString validNotes2 = "# -";
        QVector<QColor> m_columnColors;
        float m_size = 1.5;
        int m_curLinePos;
        TTRView(QWidget *parent = 0);
        ~TTRView();

        void setWidgetResizable(bool) {

        }

        void setWidget(QWidget* w) {

        }

        CIniFile* m_colors = nullptr;

        int DISPLAY_DATA_PER_LINE = 4;
        int BYTES_PER_LINE = 6;
        void Calculate();

        bool m_isChanged = false;
        QString UnpackLine(QByteArray& d, int pos );
        void PackLine(QByteArray& d, int pos, QString line);
signals:
        void emitSave();

    public slots:
        void setData(QSharedPointer<DataStorage> pData);
        void clear();
        void showFromOffset(std::size_t offset);

    protected:
        void paintEvent(QPaintEvent *event);
        void paintEventOld(QPaintEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

public:
        QSharedPointer<DataStorage>          m_pdata;
private:
        std::size_t           m_posAddr;
        std::size_t           m_posHex;
        std::size_t           m_posAscii;
        std::size_t           m_charWidth;
        std::size_t           m_charHeight;
        std::size_t           m_hexPause;


        std::size_t           m_selectBegin;
        std::size_t           m_selectEnd;
        std::size_t           m_selectInit;
        std::size_t           m_cursorPos;


        QSize fullSize() const;
        void resetSelection();
        void resetSelection(int pos);
        void setSelection(int pos);
        void ensureVisible();
        void setCursorPos(int pos);
        std::size_t cursorPos(const QPoint &position);

};

#endif