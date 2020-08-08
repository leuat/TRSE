#include "lcolor.h"

QWidget* LPen::CreateUI(QColor col, int width,int xx,int yy,QVector<LColor>& list) {

    QWidget* widget = nullptr;
    if (m_type == FixedSingle)
        widget = createButton(col, m_colorIndex, width);
    if (m_type == Dropdown || m_type == DropDownExceptAlreadySelected)
        widget = createComboBox(col, width, list);

    if (m_type == DisplayAllExceptAlreadySelected || m_type == DisplayAllExceptAlreadySelected)
        widget = createGrid(col, width,list);


    if (widget == nullptr) {
        qDebug() << "Error in LPen::CreateUI : widget is zero. Should not happen!";
    }

    return widget;

    //m_buttonsEdit.append(b);

}

QPixmap LPen::CreateColorIcon(QColor col, int s) {
    QImage img(s,s,QImage::Format_RGB32);

    for (int y=0;y<s;y++)
        for (int x=0;x<s;x++) {
            //            if (s-1-y>x)

            //              img.setPixelColor(x,y, m_list[col].color);
            //        else
            img.setPixelColor(x,y, col);


        }
    return QPixmap::fromImage(img);

}

QWidget *LPen::createButton(QColor col, int index, int width) {
    QPushButton *b = new QPushButton();
    QPalette p;
    b->setFlat(true);
    QPixmap pm = CreateColorIcon(col,width);
    b->setAutoFillBackground(true);
    p.setBrush(b->backgroundRole(), QBrush(pm));
    b->setMaximumWidth(width);
    b->setMinimumWidth(width);
    b->setMaximumHeight(width);
    b->setMinimumHeight(width);
    b->setPalette(p);
    QObject::connect( b, &QPushButton::clicked,  [=](){
        handleButtonEdit(index,b);
    } );
    return b;
}

QWidget *LPen::createComboBox(QColor col, int width, QVector<LColor> &list)
{
    QGridLayout* ly = new QGridLayout();

    QWidget* btn = createButton(col,m_colorIndex,width);

    QComboBox *b = new QComboBox();
    if (m_type == Dropdown || m_type == DisplayAll)
        FillComboBox(b, list);
    else
        FillComboBoxRestricted(b,list);

    b->setCurrentIndex(m_colorIndex);

    QObject::connect( b, &QComboBox::currentTextChanged,  [=](){
        Data::data.currentColor = b->currentData(Qt::UserRole).toInt();
        Data::data.currentIsColor=true;
        m_colorIndex = Data::data.currentColor;
//        qDebug() << "SETTING CMB COLOR INDEX " <<Get();


        Data::data.UpdatePens();

    } );

    ly->addWidget(btn,0,0);
    ly->addWidget(b,0,1);

    QWidget* dummy = new QWidget();
    dummy->setLayout(ly);

    return dummy;

}

QWidget *LPen::createGrid(QColor col, int width, QVector<LColor> &list)
{
    QGridLayout* ly = new QGridLayout();
    int j=0;
    int x = 0;
    for (int i=0;i<list.count();i++) {
        bool ok=true;

        if (m_type==DisplayAllExceptAlreadySelected) {
            for (int j=0;j<m_pens->count();j++)
                if (m_pens->at(j)->m_isHidden==false)
                if (m_pens->at(j)->m_colorIndex==i && m_pens->at(j)!=this)
                    ok = false;

        }
//        qDebug() << "REST" <<m_restricted;
        if (m_restricted.contains(i))
            ok = false;

        if (ok) {
            ly->addWidget(createButton(list[i].color,i,width),j,x);
        }
        j++;
        if (j==8) {
            j=0;x++;
        }

    }
    QWidget* w = new QWidget();
    w->setLayout(ly);
    return w;
}

void LPen::FillComboBox(QComboBox *cmb, QVector<LColor> &list)
{
    cmb->clear();
    for (int i=0;i<list.count();i++) {
        QPixmap pixmap(16,16);
        pixmap.fill(list[i].color);
        QIcon icon(pixmap);
        if (!m_restricted.contains(i))
            cmb->addItem(icon,Util::numToHex(i),i);


    }
}

void LPen::FillComboBoxRestricted(QComboBox *cmb, QVector<LColor> &list)
{
    cmb->clear();
    for (int i=0;i<list.count();i++) {
        bool ok = true;
        for (int j=0;j<m_pens->count();j++)
            if (m_pens->at(j)->m_isHidden==false)
                if (m_pens->at(j)->m_colorIndex==i && m_pens->at(j)!=this)
                ok = false; // Already used, restricted

        if (m_restricted.contains(i))
            ok = false;

        if (ok) {
            QPixmap pixmap(16,16);
            pixmap.fill(list[i].color);
            QIcon icon(pixmap);
            cmb->addItem(icon,Util::numToHex(i),i);
        }
    }
}

void LPen::handleButtonEdit(int val, QPushButton *btn)
{
    /*    for (int i=0;i<m_buttonsEdit.count();i++)
            m_buttonsEdit[i]->setText("");
        //if (data<m_buttonsEdit.count())
    //    qDebug() << data;
            m_buttonsEdit[data]->setText("X");*/
    Data::data.currentColor = val;
    Data::data.currentIsColor=true;
    Data::data.UpdatePens();
//    qDebug() << "Setting color "<< val;
    //        SetMulticolor(3,val);
    QPoint p = (QCursor::pos() - btn->mapToGlobal(QPoint(0,0)));
    QPointF fp = QPointF(p.x()/(float)btn->width(),
                         p.y()/(float)btn->height());
    //    qDebug() << (QCursor::pos() - btn->mapToGlobal(QPoint(0,0))) << btn->mapToGlobal(QPoint(0,0))  <<m_buttonsEdit[data]->cursor().pos();
    //    qDebug() <<fp;
    // Select alternative colour
    /*if (!m_list[val].ignoreAltColour)
        if (m_list[val].m_altColour!=-1)
            if (1-fp.y()<fp.x())
                Data::data.currentColor = m_list[val].m_altColour;
*/
  //  Data::data.currentColor = m_colorIndex;//  m_list[val].m_altColour;

}

