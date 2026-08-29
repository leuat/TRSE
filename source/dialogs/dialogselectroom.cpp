#include "dialogselectroom.h"
#include "ui_dialogselectroom.h"

DialogSelectRoom::DialogSelectRoom(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSelectRoom)
{
    ui->setupUi(this);
}

DialogSelectRoom::~DialogSelectRoom()
{
    delete ui;
}

void DialogSelectRoom::SetLevel(ImageLevelEditor *le)
{
    QVector<QPixmap> icons = le->CreateIcons();


    Util::clearLayout(ui->gridLevels);
    ui->gridLevels->setSpacing(0);
    ui->gridLevels->setContentsMargins(QMargins());
    //    ui->gridLevels->set
    float scale = le->m_meta.m_sizex/2;
    for (int i=0;i<le->m_meta.m_sizex;i++)
        for (int j=0;j<le->m_meta.m_sizey;j++) {
            QPushButton* l = new QPushButton();
            l->setText("");
//            l->setFixedSize(80,64);
            QPixmap icon(icons[j + i*le->m_meta.m_sizey]);
            //l->setIcon(icon.scaled(320,200,Qt::KeepAspectRatio));
            l->setIcon(icon);
            //l->setScaledContents(true);
            //l->setIconSize(QSize(80,64));
            l->setFlat(true);
            l->setIconSize(icon.rect().size()/scale);
            l->setStyleSheet("margin: 0px;");
            ui->gridLevels->addWidget(l, j,i);

            QObject::connect( l, &QPushButton::clicked,  [=](){
                m_level = QPoint(i,j);
                done(QDialog::Accepted);
            });



        }

}
