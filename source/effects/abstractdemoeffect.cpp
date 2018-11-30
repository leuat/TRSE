#include "abstractdemoeffect.h"

AbstractDemoEffect::AbstractDemoEffect(QGridLayout* gl)
{
    m_img = QImage(320,200,QImage::Format_ARGB32);
    m_gl = gl;
}

void AbstractDemoEffect::Save(QString file)
{

}

void AbstractDemoEffect::ConvertToC64()
{
    if (m_toggleC64) {
        m_mc->m_colorList.EnableColors(m_cols);
        m_mc->SetColor(m_cols[0],0);
        m_mc->SetColor(m_cols[1],1);
        m_mc->SetColor(m_cols[3],2);
        m_mc->SetColor(m_cols[2],3);
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList);

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));
    }

}

void AbstractDemoEffect::FillToGUI()
{
    while (m_gl->count()) {
        QLayoutItem * cur_item = m_gl->takeAt(0);
              if(cur_item->widget())
                  delete cur_item->widget();
              delete cur_item;
    }
    int i=0;
    for (QString s: m_params.keys()) {
        QLabel* l= new QLabel(s);
        m_gl->addWidget(l,i,0);
        QLineEdit* le= new QLineEdit(QString::number(m_params[s].m_val));
        connect(le, SIGNAL(textChanged(const QString &)), this, SLOT(FillFromGUI()));
        m_gl->addWidget(le,i,1);
        i++;
    }
//    exit(1);
}

void AbstractDemoEffect::FillFromGUI()
{
    for (int i=0;i<m_gl->rowCount();i++) {
        QLabel* lbl = dynamic_cast<QLabel*>(m_gl->itemAtPosition(i,0)->widget());
        QString s = lbl->text();
        bool ok=false;
        float val = ((QLineEdit*)m_gl->itemAtPosition(i,1)->widget())->text().toFloat(&ok);
        if (ok)
            m_params[s].m_val = val;

    }

}

