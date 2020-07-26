/*
 *
 * Hexview is originally based on https://github.com/virinext/QHexView
 * by Victor Anjin. Extended by Leuat.
 *
 *
 *
 * */

#ifndef Q_HEX_VIEWER_H_
#define Q_HEX_VIEWER_H_

#include <QAbstractScrollArea>
#include <QByteArray>
#include <QFile>
#include "source/trsedocuments/trsedocument.h"
#include "source/LeLib/util/cinifile.h"

class HexView: public QAbstractScrollArea

{
    public:
        TRSEDocument* m_doc = nullptr;

        class DataStorage
        {
            public:
                virtual ~DataStorage() {};
                virtual QByteArray getData(std::size_t position, std::size_t length) = 0;
                virtual void setData(std::size_t position, unsigned char val) = 0;
                virtual std::size_t size() = 0;
            QByteArray    m_data;
        };


        class DataStorageArray: public DataStorage
        {
            public:
                DataStorageArray(const QByteArray &arr);
                virtual void setData(std::size_t position, unsigned char val);
                virtual QByteArray getData(std::size_t position, std::size_t length);
                virtual std::size_t size();
        };

        class DataStorageFile: public DataStorage
        {
            public:
                DataStorageFile(const QString &fileName);
                virtual void setData(std::size_t position, unsigned char val);
                virtual QByteArray getData(std::size_t position, std::size_t length);
                virtual std::size_t size();
            private:
                QFile      m_file;
        };


        QString valid = "abcdef0123456789";

        HexView(QWidget *parent = 0);
        ~HexView();

        void setWidgetResizable(bool) {

        }

        void setWidget(QWidget* w) {

        }

        CIniFile* m_colors = nullptr;


    public slots:
        void setData(DataStorage *pData);
        void clear();
        void showFromOffset(std::size_t offset);

    protected:
        void paintEvent(QPaintEvent *event);
        void keyPressEvent(QKeyEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void mousePressEvent(QMouseEvent *event);

public:
        DataStorage          *m_pdata;
private:
        std::size_t           m_posAddr;
        std::size_t           m_posHex;
        std::size_t           m_posAscii;
        std::size_t           m_charWidth;
        std::size_t           m_charHeight;


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
