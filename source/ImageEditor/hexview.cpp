#include "hexview.h"

#include <QScrollBar>
#include <QPainter>
#include <QSize>
#include <QPaintEvent>
#include <QKeyEvent>
#include <QClipboard>
#include <QApplication>
#include "source/LeLib/util/util.h"
#include <QDebug>

#include <stdexcept>

const int HEXCHARS_IN_LINE = 47;
const int GAP_ADR_HEX = 10;
const int GAP_HEX_ASCII = 16;


HexView::HexView(QWidget *parent):
QAbstractScrollArea(parent),
m_pdata(NULL)
{
    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().averageCharWidth();//.width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();
    Calculate();
    setFocusPolicy(Qt::StrongFocus);
}


HexView::~HexView()
{
    if(m_pdata)
        delete m_pdata;
}

void HexView::Calculate()
{
    m_hexPause = 00;
    m_posAddr = 0;
    m_posHex = 10 * m_charWidth + GAP_ADR_HEX;
    m_posAscii = m_posHex + (1+BYTES_PER_LINE)*3 * m_charWidth + GAP_HEX_ASCII + m_hexPause;

    setMinimumWidth(m_posAscii + (BYTES_PER_LINE * m_charWidth));

}

void HexView::setData(HexView::DataStorage *pData)
{
    verticalScrollBar()->setValue(0);
    if(m_pdata)
        delete m_pdata;
    m_pdata = pData;
    m_cursorPos = 0;
    resetSelection(0);
}


void HexView::showFromOffset(std::size_t offset)
{
    if(m_pdata && offset < m_pdata->size())
    {
        setCursorPos(offset * 2);

        int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

        verticalScrollBar() -> setValue(cursorY);
    }
}

void HexView::clear()
{
    verticalScrollBar()->setValue(0);
}


QSize HexView::fullSize() const
{
    if(!m_pdata)
        return QSize(0, 0);

    std::size_t width = m_posAscii + (BYTES_PER_LINE * m_charWidth);
    std::size_t height = m_pdata->size() / BYTES_PER_LINE;
    if(m_pdata->size() % BYTES_PER_LINE)
        height++;

    height *= m_charHeight;

    return QSize(width, height);
}

void HexView::paintEvent(QPaintEvent *event)
{
    if(!m_pdata)
        return;
    Calculate();
    QPainter painter(viewport());
    float dy = m_size;
    float dx = dy;

    setFont(QFont("Courier", 10.0*dy));

    m_charWidth = fontMetrics().averageCharWidth();//.width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();
    dx = m_charHeight/10.0;
    dy = m_charHeight/10.0;
    Calculate();

    QSize areaSize = viewport()->size();
    QSize  widgetSize = fullSize();
    verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
    verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);

    int firstLineIdx = verticalScrollBar() -> value();

    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
    if(lastLineIdx > m_pdata->size() / BYTES_PER_LINE)
    {
        lastLineIdx = m_pdata->size() / BYTES_PER_LINE;
        if(m_pdata->size() % BYTES_PER_LINE)
            lastLineIdx++;
    }
    painter.setFont(font());
    painter.fillRect(event->rect(), m_colors->getColor("backgroundcolor"));

    QColor addressAreaColor = QColor(m_colors->getColor("linenumbersbackground"));
    painter.fillRect(QRect(m_posAddr, event->rect().top(), m_posHex - GAP_ADR_HEX + 2 , height()), addressAreaColor);

    int linePos = m_posAscii - (GAP_HEX_ASCII / 2);
    painter.setPen(m_colors->getColor("cycles"));

    painter.drawLine(linePos, event->rect().top(), linePos, height());

    painter.setPen(m_colors->getColor("textcolor"));

    int yPosStart = m_charHeight;

    QBrush def = painter.brush();
    QBrush selected = QBrush(m_colors->getColor("currentline"));
    QColor textForeground = (m_colors->getColor("textcolor"));
    QColor textSelected = (m_colors->getColor("numberscolor"));
    QByteArray data = m_pdata->getData(firstLineIdx * BYTES_PER_LINE, (lastLineIdx - firstLineIdx) * BYTES_PER_LINE);

    for (int lineIdx = firstLineIdx, yPos = yPosStart;  lineIdx < lastLineIdx; lineIdx += 1, yPos += m_charHeight)
    {
        painter.setPen(m_colors->getColor("numberscolor"));
        QString address = QString("%1").arg(lineIdx * BYTES_PER_LINE, 10, 16, QChar('0'));
        painter.drawText(m_posAddr, yPos, address);

        painter.setPen(textForeground);

        for(int xPos = m_posHex, i=0; i<BYTES_PER_LINE && ((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) < data.size(); i++, xPos += 3 * m_charWidth)
        {
            std::size_t pos = (lineIdx * BYTES_PER_LINE + i) * 2;
            painter.setBackground(def);
            painter.setBackgroundMode(Qt::OpaqueMode);
            painter.setPen(textForeground);
            if(pos >= m_selectBegin && pos < m_selectEnd)
            {
                painter.setBackground(selected);
                painter.setBackgroundMode(Qt::OpaqueMode);
                painter.setPen(textSelected);
            }
            if (m_selectBegin==m_selectEnd) { // Show current pos only
                if ((pos)/2 == ((m_cursorPos)/2)) {
                    painter.setBackground(selected);
                    painter.setPen(textSelected);
                }
            }

            QString val = QString::number((data.at((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) & 0xF0) >> 4, 16);
            painter.drawText(xPos, yPos, val);

            if((pos+1) >= m_selectBegin && (pos+1) < m_selectEnd)
            {
                painter.setBackground(selected);
                painter.setBackgroundMode(Qt::OpaqueMode);
                painter.setPen(textSelected);
            }
            else
            {
                painter.setBackground(def);
                painter.setBackgroundMode(Qt::OpaqueMode);
                painter.setPen(textForeground);
            }
            if (m_selectBegin==m_selectEnd) { // Show current pos only
                if ((pos+1)/2 == ((m_cursorPos)/2)) {
                    painter.setBackground(selected);
                    painter.setPen(textSelected);
                }
            }

            val = QString::number((data.at((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) & 0xF), 16);
            painter.drawText(xPos+m_charWidth, yPos, val);

            painter.setBackground(def);
            painter.setBackgroundMode(Qt::OpaqueMode);

            if (i==8) xPos+=m_hexPause;


        }
        painter.setPen(m_colors->getColor("symbolscolor"));

        for (int xPosAscii = m_posAscii, i=0; ((lineIdx - firstLineIdx) * BYTES_PER_LINE + i) < data.size() && (i < BYTES_PER_LINE); i++, xPosAscii += m_charWidth)
        {
            int pos = (lineIdx - firstLineIdx) * BYTES_PER_LINE + i;
            if (pos>=data.count())
                break;

            char ch = data[pos];
            if ((ch < 0x20) || (ch > 0x7e))
            ch = '.';
            pos = (lineIdx * BYTES_PER_LINE + i) * 2;
            painter.setBackground(def);
            painter.setPen(textForeground);

            if(((pos+1) >= m_selectBegin && (pos+1) < m_selectEnd)) {
                painter.setBackground(selected);
                painter.setPen(textSelected);
            }

            if (m_selectBegin==m_selectEnd) { // Show current pos only
                if (pos == (m_cursorPos&0xFFFFFFFE)) {
                    painter.setBackground(selected);
                    painter.setPen(textSelected);

                }
            }

            painter.drawText(xPosAscii, yPos, QString(ch));
        }

        int sx = m_posAscii + BYTES_PER_LINE * m_charWidth + GAP_HEX_ASCII;;
        painter.setBackground(def);

        painter.setPen(m_colors->getColor("cycles"));

        painter.drawLine(sx-8, event->rect().top(), sx-8, height());


        painter.setPen(m_colors->getColor("quotationcolor"));
        painter.setBrush(m_colors->getColor("quotationcolor"));
        float ddx = dx;
        float ddy = dy;
        for (int i=0;i<BYTES_PER_LINE/8;i++)
        {

            for (int b=0;b<8;b++) {
                int pos = (lineIdx - firstLineIdx) * BYTES_PER_LINE + b +i*8;
                if (pos<data.count()) {
                    uchar v = data[pos];

                    for (int x=0;x<8;x++) {
                        int j = 7-x;
                        if (((v>>j)&1)==1)
                            painter.drawRect(QRect(sx+dx*x + i*(m_charWidth*1.5)*ddx, yPos+ddy*b-m_charHeight*0.75, ddx,ddy));

                    }
                }
            }
       }


    }

    if (hasFocus())
    {
        int x = (m_cursorPos % (2 * BYTES_PER_LINE));
        int y = m_cursorPos / (2 * BYTES_PER_LINE);
        y -= firstLineIdx;
        int cursorX = (((x / 2) * 3) + (x % 2)) * m_charWidth + m_posHex;
        int cursorY = y * m_charHeight + 4;
        painter.fillRect(cursorX, cursorY, 2, m_charHeight, m_colors->getColor("textcolor"));
    }
}


void HexView::keyPressEvent(QKeyEvent *event)
{
    bool setVisible = false;
    if (m_doc!=nullptr)
        m_doc->keyPressEvent(event);
/*****************************************************************************/
/* Cursor movements */
/*****************************************************************************/
    if(event->matches(QKeySequence::MoveToNextChar))
    {
        setCursorPos(m_cursorPos + 1);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousChar))
    {
        setCursorPos(m_cursorPos - 1);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToEndOfLine))
    {
        setCursorPos(m_cursorPos | ((BYTES_PER_LINE * 2) - 1));
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToStartOfLine))
    {
        setCursorPos(m_cursorPos | (m_cursorPos % (BYTES_PER_LINE * 2)));
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousLine))
    {
        setCursorPos(m_cursorPos - BYTES_PER_LINE * 2);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToNextLine))
    {
        setCursorPos(m_cursorPos + BYTES_PER_LINE * 2);
        resetSelection(m_cursorPos);
        setVisible = true;
    }

    if(event->matches(QKeySequence::MoveToNextPage))
    {
        setCursorPos(m_cursorPos + (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousPage))
    {
        setCursorPos(m_cursorPos - (viewport()->height() / m_charHeight - 1) * 2 * BYTES_PER_LINE);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToEndOfDocument))
    {
        if(m_pdata)
            setCursorPos(m_pdata->size() * 2);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToStartOfDocument))
    {
        setCursorPos(0);
        resetSelection(m_cursorPos);
        setVisible = true;
    }

/*****************************************************************************/
/* Select commands */
/*****************************************************************************/
    if (event->matches(QKeySequence::SelectAll))
    {
        resetSelection(0);
        if(m_pdata)
            setSelection(2 * m_pdata->size() + 1);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectNextChar))
    {
        int pos = m_cursorPos + 1;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousChar))
    {
        int pos = m_cursorPos - 1;
        setSelection(pos);
        setCursorPos(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectEndOfLine))
    {
        int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE)) + (2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectStartOfLine))
    {
        int pos = m_cursorPos - (m_cursorPos % (2 * BYTES_PER_LINE));
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousLine))
    {
        int pos = m_cursorPos - (2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectNextLine))
    {
        int pos = m_cursorPos + (2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }

    if (event->matches(QKeySequence::SelectNextPage))
    {
        int pos = m_cursorPos + (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectPreviousPage))
    {
        int pos = m_cursorPos - (((viewport()->height() / m_charHeight) - 1) * 2 * BYTES_PER_LINE);
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectEndOfDocument))
    {
        int pos = 0;
        if(m_pdata)
            pos = m_pdata->size() * 2;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }
    if (event->matches(QKeySequence::SelectStartOfDocument))
    {
        int pos = 0;
        setCursorPos(pos);
        setSelection(pos);
        setVisible = true;
    }



    if (event->matches(QKeySequence::Copy))
    {
        if(m_pdata)
        {
            QString res;
            int idx = 0;
            int copyOffset = 0;

            QByteArray data = m_pdata->getData(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
            if(m_selectBegin % 2)
            {
                res += QString::number((data.at((idx+1) / 2) & 0xF), 16);
                res += " ";
                idx++;
                copyOffset = 1;
            }

            int selectedSize = m_selectEnd - m_selectBegin;
            for (;idx < selectedSize; idx+= 2)
            {
                QString val = QString::number((data.at((copyOffset + idx) / 2) & 0xF0) >> 4, 16);
                if(idx + 1 < selectedSize)
                {
                    val += QString::number((data.at((copyOffset + idx) / 2) & 0xF), 16);
                    val += " ";
                }
                res += val;

                if((idx/2) % BYTES_PER_LINE == (BYTES_PER_LINE - 1))
                    res += "\n";
            }
            QClipboard *clipboard = QApplication::clipboard();
            clipboard -> setText(res);
        }
    }


    QString t = event->text();

    if(event->key() == Qt::Key_Delete)
    {
        m_pdata->m_data.remove(m_selectBegin / 2, (m_selectEnd - m_selectBegin) / 2 + 1);
        m_isChanged = true;
    }

    if(event->key() == Qt::Key_I)
    {
        m_pdata->Insert(m_cursorPos/2,0);
        m_isChanged = true;
    }


    if (valid.contains(t) && t!="") {
        uchar val = m_pdata->getData(m_cursorPos/2,1)[0];
        bool ok;
        int shift = 0;
        if ((m_cursorPos&1)==0) {
            val = val & 0x0F;
            shift = 4;
        }
        else val = val & 0xF0;
        val = val | t.toInt(&ok,16)<<shift;

        if (m_pdata!=nullptr)
           m_pdata->setData(m_cursorPos/2,val);

//        qDebug() << Util::numToHex(val) << t;
        setCursorPos(m_cursorPos + 1);
        resetSelection(m_cursorPos);
        setVisible = true;
        m_isChanged = true;
    }






    if(setVisible)
        ensureVisible();

    viewport() -> update();
}

void HexView::mouseMoveEvent(QMouseEvent * event)
{
    int actPos = cursorPos(event->pos());
    setCursorPos(actPos);
    setSelection(actPos);

    viewport() -> update();
}

void HexView::mousePressEvent(QMouseEvent * event)
{
    int cPos = cursorPos(event->pos());

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event -> button() == Qt::LeftButton)
        setSelection(cPos);
    else
        resetSelection(cPos);

    setCursorPos(cPos);

    viewport() -> update();
}


std::size_t HexView::cursorPos(const QPoint &position)
{
    int pos = -1;

    if ((position.x() >= m_posHex) && (position.x() < (m_posHex + HEXCHARS_IN_LINE * m_charWidth)))
    {
        int x = (position.x() - m_posHex) / m_charWidth;
        if ((x % 3) == 0)
            x = (x / 3) * 2;
        else
            x = ((x / 3) * 2) + 1;

        int firstLineIdx = verticalScrollBar() -> value();
        int y = (position.y() / m_charHeight) * 2 * BYTES_PER_LINE;
        pos = x + y + firstLineIdx * BYTES_PER_LINE * 2;
    }
    return pos;
}


void HexView::resetSelection()
{
    m_selectBegin = m_selectInit;
    m_selectEnd = m_selectInit;
}

void HexView::resetSelection(int pos)
{
    if (pos < 0)
        pos = 0;

    m_selectInit = pos;
    m_selectBegin = pos;
    m_selectEnd = pos;
}

void HexView::setSelection(int pos)
{
    if (pos < 0)
        pos = 0;

    if (pos >= m_selectInit)
    {
        m_selectEnd = pos;
        m_selectBegin = m_selectInit;
    }
    else
    {
        m_selectBegin = pos;
        m_selectEnd = m_selectInit;
    }
}


void HexView::setCursorPos(int position)
{
    if(position < 0)
        position = 0;

    int maxPos = 0;
    if(m_pdata)
    {
        maxPos = m_pdata->size() * 2;
        if(m_pdata->size() % BYTES_PER_LINE)
            maxPos++;
    }

    if(position > maxPos)
        position = maxPos;

    m_cursorPos = position;
}

void HexView::ensureVisible()
{
    QSize areaSize = viewport()->size();

    int firstLineIdx = verticalScrollBar() -> value();
    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;

    int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);

    if(cursorY < firstLineIdx)
        verticalScrollBar() -> setValue(cursorY);
    else if(cursorY >= lastLineIdx)
        verticalScrollBar() -> setValue(cursorY - areaSize.height() / m_charHeight + 1);
}



HexView::DataStorageArray::DataStorageArray(const QByteArray &arr)
{
    m_data = arr;
}

void HexView::DataStorageArray::setData(std::size_t position, unsigned char val)
{
//    if (position<m_data.length())
        m_data[(int)position] = val;
}

QByteArray HexView::DataStorageArray::getData(std::size_t position, std::size_t length)
{
    return m_data.mid(position, length);
}


std::size_t HexView::DataStorageArray::size()
{
    return m_data.count();
}


HexView::DataStorageFile::DataStorageFile(const QString &fileName): m_file(fileName)
{
    m_file.open(QIODevice::ReadOnly);
    if(!m_file.isOpen())
        throw std::runtime_error(std::string("Failed to open file `") + fileName.toStdString() + "`");
}

void HexView::DataStorageFile::setData(std::size_t position, unsigned char val)
{

}

QByteArray HexView::DataStorageFile::getData(std::size_t position, std::size_t length)
{
    m_file.seek(position);
    return m_file.read(length);
}


std::size_t HexView::DataStorageFile::size()
{
    return m_file.size();
}
