#include "lcolor.h"

QWidget* LPen::CreateUI(QColor col, int width,int xx,int yy,QVector<LColor>& list) {

    QWidget* widget = nullptr;
    if (m_type == FixedSingle)
        widget = createButton(col, m_colorIndex, width);

    if (m_type == SingleSelect)
        widget = createButtonSelect(col, m_colorIndex, width);

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

    QColor c2(col.red()/2, col.green()/2, col.blue()/2);
    for (int y=0;y<s;y++)
        for (int x=0;x<s;x++) {
            //            if (s-1-y>x)

            //              img.setPixelColor(x,y, m_list[col].color);
            //        else
            if (y==0 || y==s-1 ||x==0 || x==s-1)
                img.setPixelColor(x,y, c2);
            else
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

    QColor c2(0,0,0);
    if (col.red()+col.green()+col.blue()<(127*3))
        c2 = QColor(255,255,255);
    p.setColor(b->foregroundRole(),c2);
    b->setPalette(p);
    if (index == Data::data.currentColor)
        b->setText("X");

//    b->setStyleSheet("QLabel { color : " + c2.name() +"; }");
    QObject::connect( b, &QPushButton::clicked,  [=](){
        handleButtonEdit(index,b);
    } );
    return b;
}

QWidget *LPen::createButtonSelect(QColor col, int index, int width)
{
    QWidget* w = new QWidget();
    QGridLayout* ly = new QGridLayout();

    ly->addWidget(createButton(col,index,width),0,0);

    QPushButton* palSelect = new QPushButton("..");
    int w2= width/2;
    palSelect->setMaximumSize(w2,w2);
    ly->addWidget(createButton(col,index,width),0,0);
    ly->addWidget(palSelect,0,1);


    QObject::connect( palSelect, &QPushButton::clicked,  [=](){
        // Implement!
    } );


    w->setLayout(ly);

    return w;
}

QWidget *LPen::createComboBox(QColor col, int width, QVector<LColor> &list)
{
    QGridLayout* ly = new QGridLayout();
    ly->setSpacing(0);
    ly->setVerticalSpacing(0);
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

    QGridLayout* ly_small = new QGridLayout();
    ly_small->setVerticalSpacing(0);

    ly_small->addWidget(new QLabel(m_name),0,0);
    ly_small->addWidget(b,1,0);
    ly_small->setSpacing(0);

    QWidget* w_small = new QWidget();
    w_small->setLayout(ly_small);
//    w_small->setMaximumSize(1000,width);

    ly->addWidget(w_small,0,1);

    QWidget* dummy = new QWidget();
    dummy->setLayout(ly);
    ly->setMargin(4);
    ly_small->setMargin(0);
//    dummy->setMaximumSize(1000,width*1);
    return dummy;

}

QWidget *LPen::createGrid(QColor col, int width, QVector<LColor> &list)
{
    QGridLayout* ly = new QGridLayout();
    int j=0;
    int x = 0;
    bool filled = false;
    ly->addWidget(new QLabel(m_name),0,0);
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
            ly->addWidget(createButton(list[i].color,i,width),j+1,x);
            if (x>=1) filled = true;

        }
        j++;
        if (j==8) {
            j=0;x++;
        }

    }
    if (!filled) {
        QWidget* dummy = new QWidget();
        dummy->setMaximumSize(width,width);
        ly->addWidget(dummy,0,1);
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

