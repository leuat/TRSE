#include "ttrview.h"

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


TTRView::TTRView(QWidget *parent):
QAbstractScrollArea(parent),
m_pdata(NULL)
{
    setFont(QFont("Courier", 10));

    m_charWidth = fontMetrics().averageCharWidth();//.width(QLatin1Char('9'));
    m_charHeight = fontMetrics().height();
    Calculate();
    setFocusPolicy(Qt::StrongFocus);
}


TTRView::~TTRView()
{
}


void TTRView::Calculate()
{
    m_hexPause = 02;
    m_posAddr = 0;
    m_posHex = 4 * m_charWidth + GAP_ADR_HEX;
    //    m_posAscii = m_posHex + (1+BYTES_PER_LINE)*3 * m_charWidth + GAP_HEX_ASCII + m_hexPause;

    //  qDebug() <<
    //    setMinimumWidth(m_posAscii + (BYTES_PER_LINE * m_charWidth));

}

QString TTRView::UnpackLine(QByteArray &d, int pos)
{
    if (pos+4>d.count())
        return "";
//    qDebug() << "unpackline : "<<pos;
    QByteArray line = d.mid(pos,4);
    int semitone = 0;
    int note = -1;
    int octave = 0;
    int volume = 0;
    int instr = 0;
    int command = 0;
    int param = 0;

    QString s = "";
    if ((line[0] & 0x80)) {
        semitone = line[0] & 0x7f;
        if (semitone == 0x7f)
            note = -1;
        else {
            note = semitone % 12;
            octave = semitone / 12;
        }
    }

    if (line[1] & 0x80)
      volume = line[1] & 0x3f;

    if (line[1] & 0x40)
        instr = (line[2] & 0xf0) >> 4;

    command = line[2] & 0x0f;
    param = (uchar)line[3];


//            return (note, octave, instr, volume, command, param)

    QString oct = QString("%1").arg(octave, 2, 16, QChar('0'));
    QString inst = QString("%1").arg(instr, 2, 16, QChar('0'));
    QString vol = QString("%1").arg(volume, 2, 16, QChar('0'));
    QString cmd = QString("%1").arg(command, 2, 16, QChar('0'));
    QString par = QString("%1").arg(param, 2, 16, QChar('0'));
    QString n = "--";
    if (note!=-1)
        n = notes[note];
    QString l = n + " " + oct + " "+inst + " " +vol + " " +cmd + " " +par;

    return l;

    //    return Util::toString(Util::ByteArrayToHexQStringList(line));
}

void TTRView::PackLine(QByteArray &d, int pos, QString line)
{
    QStringList lst = line.trimmed().simplified().split(" ");
    //if (lst[0]!="--" )
    int note = -1;
    lst[0] = lst[0].toUpper();
    if (lst[0].count()==1) lst[0]+="-";
    if (notes.contains(lst[0]))
        note = notes.indexOf(lst[0]);


    //QString l = n + " " + oct + " " +vol + " " +cmd + " " +par;

    bool ok;
    lst[1][0] = '0';
    int octave = lst[1].toInt(&ok,16);
    if (octave>9)
        octave = 9;

    if (!ok)
        octave = -1;

    int inst = lst[2].toInt(&ok,16);
    if (!ok)
        inst = -1;
    int vol = lst[3].toInt(&ok,16);
    if (!ok) vol = -1;
    int cmd = lst[4].toInt(&ok,16);
    if (!ok) cmd = -1;

    int par = lst[5].toInt(&ok,16);
    if (!ok) par = -1;



    for (int i=0;i<DISPLAY_DATA_PER_LINE;i++)
        d[pos+i]=0;

    if (note!=-1 && octave!=-1) {
        int semitone = octave*12 + note;
        d[pos] = 0x80 | semitone;
    }
//    qDebug() << note << octave <<Util::numToHex(d[pos]);
    if (vol!=-1)
        d[pos+1] = (uchar)0x80 | (uchar)vol;

    if (inst!=-1) {
        d[pos+1] = d[pos+1]|(uchar)0x40;
        d[pos+2] = ((uchar)inst&0xf)<<4;
    }
    if (note==-1)
        d[pos+1]=0;
    if (cmd!=-1 && par!=-1) {
        d[pos+2] = d[pos+2]| (((uchar)cmd)&0xf);
        d[pos+3] = (uchar)par;
    }

  /*  qDebug() << "AFTER";
    for (int i=0;i<4;i++)
        qDebug() << Util::numToHex((uchar)d[pos+i]);
*/
}

void TTRView::setData(QSharedPointer<DataStorage> pData)
{
    m_pdata = pData;
    verticalScrollBar()->setValue(0);
    m_cursorPos = 0;
    resetSelection(0);
}


void TTRView::showFromOffset(std::size_t offset)
{
    if(m_pdata && offset < m_pdata->size())
    {
        setCursorPos(offset * 2);

        int cursorY = m_cursorPos / (2 * BYTES_PER_LINE);
        if (verticalScrollBar()!=nullptr)
        verticalScrollBar() -> setValue(cursorY);
    }
}

void TTRView::clear()
{
    if (verticalScrollBar()!=nullptr)
        verticalScrollBar()->setValue(0);
}



QSize TTRView::fullSize() const
{
    if(m_pdata==nullptr)
        return QSize(0, 0);

    std::size_t width = m_posAscii + (BYTES_PER_LINE * m_charWidth);
    std::size_t height = m_pdata->size() / BYTES_PER_LINE;
    if(m_pdata->size() % BYTES_PER_LINE)
        height++;

    height *= m_charHeight;

    return QSize(width, height);
}


void TTRView::paintEventOld(QPaintEvent *event) {
    if(m_pdata==nullptr)
        return;
    if (m_pdata->size()==0)
        return;
    if (m_colors==nullptr)
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
    int firstLineIdx = 0;
    if (verticalScrollBar()!=nullptr) {
        verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
        verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);
        firstLineIdx = verticalScrollBar() -> value();
    }


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

//    painter.drawLine(linePos, event->rect().top(), linePos, height());

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
//        QString address = QString("%1").arg(lineIdx * BYTES_PER_LINE, 10, 16, QChar('0'));
 //       painter.drawText(m_posAddr, yPos, address);
        QString address = QString("%1").arg(lineIdx, 2, 16, QChar('0'));
        painter.drawText(m_posAddr, yPos, address);

        painter.setPen(textForeground);
        int cnt=0;
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

//            if (i==4) xPos+=m_hexPause;
            if ((i&3)==3) xPos+=m_charWidth*2;


        }
/*        painter.setPen(m_colors->getColor("symbolscolor"));

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
        */

    }

    if (hasFocus())
    {
        int x = (m_cursorPos % (2 * BYTES_PER_LINE));
        int y = m_cursorPos / (2 * BYTES_PER_LINE);
        y -= firstLineIdx;
        int cursorX = (((x / 2) * 3) + (x % 2)) * m_charWidth + m_posHex;
        cursorX += (int)(x/8)*m_charWidth*2;
        int cursorY = y * m_charHeight + 4;
        painter.fillRect(cursorX, cursorY, 2, m_charHeight, m_colors->getColor("textcolor"));
    }
}




void TTRView::paintEvent(QPaintEvent *event)
{
    if(m_pdata==nullptr)
        return;
    if (m_pdata->size()==0)
        return;
    if (m_colors==nullptr)
        return;

    if (m_columnColors.count()==0) {
        m_columnColors.append(m_colors->getColor("builtinfunctioncolor")); // volume
        m_columnColors.append(m_colors->getColor("keywordcolor")); // octave
        m_columnColors.append(m_colors->getColor("textcolor")); // notes
        m_columnColors.append(m_colors->getColor("quotationcolor")); // instr
        m_columnColors.append(m_colors->getColor("symbolscolor")); // instr
        m_columnColors.append(m_colors->getColor("cycles")); // instr
    }
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
    int firstLineIdx = 0;
    if (verticalScrollBar()!=nullptr) {
        verticalScrollBar()->setPageStep(areaSize.height() / m_charHeight);
        verticalScrollBar()->setRange(0, (widgetSize.height() - areaSize.height()) / m_charHeight + 1);
        firstLineIdx = verticalScrollBar() -> value();
    }


    int lastLineIdx = firstLineIdx + areaSize.height() / m_charHeight;
    if(lastLineIdx > m_pdata->size() / DISPLAY_DATA_PER_LINE)
    {
        lastLineIdx = m_pdata->size() / DISPLAY_DATA_PER_LINE;
        if(m_pdata->size() % DISPLAY_DATA_PER_LINE)
            lastLineIdx++;
    }



    painter.setFont(font());
    painter.fillRect(event->rect(), m_colors->getColor("backgroundcolor"));

    QColor addressAreaColor = QColor(m_colors->getColor("linenumbersbackground"));
    painter.fillRect(QRect(m_posAddr, event->rect().top(), m_posHex - GAP_ADR_HEX + 2 , height()), addressAreaColor);

    int linePos = m_posAscii - (GAP_HEX_ASCII / 2);
    painter.setPen(m_colors->getColor("cycles"));

//    painter.drawLine(linePos, event->rect().top(), linePos, height());

    painter.setPen(m_colors->getColor("textcolor"));

    int yPosStart = m_charHeight*2;

    QBrush def = painter.brush();
    QBrush selected = QBrush(m_colors->getColor("currentline"));
//    QColor textForeground = (m_colors->getColor("textcolor"));


    QColor textSelected = (m_colors->getColor("numberscolor"));
    QByteArray data = m_pdata->getData(firstLineIdx * BYTES_PER_LINE, (lastLineIdx - firstLineIdx) * BYTES_PER_LINE);

    painter.setBackgroundMode(Qt::OpaqueMode);

    painter.setPen(textSelected);
    painter.setBackground(def);
//    QString l = n + " " + oct + " "+inst + " " +vol + " " +cmd + " " +par;
    painter.setFont(QFont("Courier", 8.0*dy));

    painter.drawText(m_posAddr, m_charHeight, "    note oct ins vol cmd par");

    painter.setFont(font());

    for (int lineIdx = firstLineIdx, yPos = yPosStart;  lineIdx < lastLineIdx; lineIdx += 1, yPos += m_charHeight)
    {
        painter.setPen(m_colors->getColor("numberscolor"));
//        QString address = QString("%1").arg(lineIdx * BYTES_PER_LINE, 10, 16, QChar('0'));
 //       painter.drawText(m_posAddr, yPos, address);
        QString address = QString("%1").arg(lineIdx, 2, 16, QChar('0'));
        painter.drawText(m_posAddr, yPos, address);

        if (((lineIdx+1)&3)==0) {
            painter.setPen(QColor(64,64,64));
            painter.drawLine(QLine(m_posAddr,yPos+3,event->rect().right(),yPos+3));
        }

        painter.setBackground(def);
        std::size_t pos = (lineIdx * DISPLAY_DATA_PER_LINE);
        QString line = UnpackLine(data, pos);


        if (hasFocus())
        if (m_cursorPos/(BYTES_PER_LINE*2)==lineIdx) {
            painter.setBackground(selected);
            m_curLinePos = pos;
//            painter.setPen(textSelected);
        }
        int xp = m_posHex;
        int lp = 0;

        if (line!="")
        for (int i=0;i<BYTES_PER_LINE;i++){
            painter.setPen(m_columnColors[i]);
            painter.drawText(xp, yPos, QString(line[lp])+QString(line[lp+1]));
            xp+=m_charWidth*3;
            lp+=3;
        }

        if (hasFocus())
        if (m_cursorPos/(BYTES_PER_LINE*2)==lineIdx) {
            painter.setBackground(selected);
            painter.setPen(textSelected);
            int pp = m_cursorPos%(BYTES_PER_LINE*2);
//            int po = pp/2;
            pp += (int)(pp/2);
            QString val = QString(line[pp]);
            painter.drawText(m_posHex+m_charWidth*pp, yPos, val);
        }





        }
/*    if (hasFocus())
    {
        int x = (m_cursorPos % (2 * BYTES_PER_LINE));
        int y = m_cursorPos / (2 * BYTES_PER_LINE);
        y -= firstLineIdx;
        int cursorX = (((x / 2) * 3) + (x % 2)) * m_charWidth + m_posHex;
        cursorX += (int)(x/8)*m_charWidth*2;
        int cursorY = y * m_charHeight + 4;
        painter.fillRect(cursorX, cursorY, 2, m_charHeight, m_colors->getColor("textcolor"));
    }
    */

}


void TTRView::keyPressEvent(QKeyEvent *event)
{
    bool setVisible = false;
    if (m_doc!=nullptr)
        m_doc->keyPressEvent(event);
/*****************************************************************************/
/* Cursor movements */
/*****************************************************************************/
    if(event->matches(QKeySequence::MoveToNextChar))
    {
        if (((m_cursorPos+1)%(BYTES_PER_LINE*2))==0) {
            emit emitMove(1,m_cursorPos-(m_cursorPos)%(BYTES_PER_LINE*2));
            return;
        }
        setCursorPos(m_cursorPos + 1);
        resetSelection(m_cursorPos);
        setVisible = true;
    }
    if(event->matches(QKeySequence::MoveToPreviousChar))
    {
        if (((m_cursorPos)%(BYTES_PER_LINE*2))==0) {
            emit emitMove(-1,m_cursorPos+((BYTES_PER_LINE*2))-1);
            return;
        }
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

    if (event->key()==Qt::Key_I)
    {
        int pos = m_cursorPos/(BYTES_PER_LINE*2)*DISPLAY_DATA_PER_LINE;
        m_pdata->m_data.remove(m_pdata->m_data.count()-DISPLAY_DATA_PER_LINE,DISPLAY_DATA_PER_LINE);
        for (int i=0;i<DISPLAY_DATA_PER_LINE;i++)
        m_pdata->m_data.insert(pos,(char)0);
        setVisible = true;
    }

//    if (event->matches(QKeySequence::Delete))
     if (event->key()==Qt::Key_Backspace)
    {
        int pos = m_cursorPos/(BYTES_PER_LINE*2)*DISPLAY_DATA_PER_LINE;
        m_pdata->m_data.remove(pos,DISPLAY_DATA_PER_LINE);
        for (int i=0;i<DISPLAY_DATA_PER_LINE;i++)
        m_pdata->m_data.insert(m_pdata->m_data.count(),(char)0);
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
/*
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

*/
    if (t!="" && (validInput.contains(t.toLower()))) {
        t = t.toLower();
        int curP = (m_cursorPos%(BYTES_PER_LINE*2));
        int curPSet = curP + (int)(curP/2);
        bool isNote =  curP<3;
        QString line = UnpackLine(m_pdata->m_data,m_curLinePos);

//        qDebug() << line;
        if (curP==0){
            if (validNotes1.contains(t))
                line[curPSet] = t[0];
        } else
        if (curP==1){
            if (validNotes2.contains(t))
                line[curPSet] = t[0];
        }
        else
            if (validHex.contains(t))
                line[curPSet] = t[0];

        if (line[1]=='-') line[1] = ' ';

        if (notesOnly.contains(line[1]))  {
            line[0] = line[1];
            line[1] = ' ';
        }

        if (!isNote) // Update keepsake data if it isn't a note
            m_lastLine = line;
        else {
            if (m_lastLine!="")
                line = line.mid(0,3) + m_lastLine.mid(3,m_lastLine.count());
        }
         PackLine(m_pdata->m_data,m_curLinePos,line);

        setCursorPos(m_cursorPos + BYTES_PER_LINE * 2);
        resetSelection(m_cursorPos);
        setVisible = true;
        m_isChanged = true;
    }






    if(setVisible)
        ensureVisible();

    viewport() -> update();
    emit emitChangeTriggered();
}

void TTRView::mouseMoveEvent(QMouseEvent * event)
{
    int actPos = cursorPos(event->pos());
    setCursorPos(actPos);
    setSelection(actPos);

    viewport() -> update();
}

void TTRView::mousePressEvent(QMouseEvent * event)
{
    int cPos = cursorPos(event->pos());

    if((QApplication::keyboardModifiers() & Qt::ShiftModifier) && event -> button() == Qt::LeftButton)
        setSelection(cPos);
    else
        resetSelection(cPos);

    setCursorPos(cPos);

    viewport() -> update();
}


std::size_t TTRView::cursorPos(const QPoint &position)
{
    int pos = -1;
    if ((position.x() >= m_posHex) && (position.x() < (m_posHex + HEXCHARS_IN_LINE * m_charWidth)))
    {
        int x = (position.x() - m_posHex) / m_charWidth;
        if ((x % 3) == 0)
            x = (x / 3) * 2;
        else
            x = ((x / 3) * 2) + 1;

        int firstLineIdx = 0;
        if (verticalScrollBar()) firstLineIdx = verticalScrollBar() -> value();
        firstLineIdx--;
        int y = (position.y() / m_charHeight) * 2 * BYTES_PER_LINE;
        pos = x + y + firstLineIdx * BYTES_PER_LINE * 2;
    }
    return pos;
}


void TTRView::resetSelection()
{
    m_selectBegin = m_selectInit;
    m_selectEnd = m_selectInit;
}

void TTRView::resetSelection(int pos)
{
    if (pos < 0)
        pos = 0;

    m_selectInit = pos;
    m_selectBegin = pos;
    m_selectEnd = pos;
}

void TTRView::setSelection(int pos)
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


void TTRView::setCursorPos(int position)
{
    if(position < 0)
        position = 0;


    int maxPos = 0;
    if(m_pdata)
    {
        maxPos = m_pdata->size() * 2/(DISPLAY_DATA_PER_LINE/(float)BYTES_PER_LINE)-1;
        //if(m_pdata->size() % BYTES_PER_LINE)
          //  maxPos++;
    }

    if(position > maxPos)
        position = maxPos;


    m_cursorPos = position;
}

void TTRView::ensureVisible()
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



