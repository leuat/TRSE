#include "dialoghelp.h"
#include "ui_dialoghelp.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

DialogHelp::DialogHelp(QWidget *parent, QString txt) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    ui->setupUi(this);
    m_helper.LoadFromResource();
    m_curTopic = 0;
    m_curItem = 0;
    FillTopics();
    FillItems(m_curTopic);
    FillHelpText();

    ui->leSearch->setText(txt);

}

DialogHelp::~DialogHelp()
{
    delete ui;
}

void DialogHelp::FillTopics()
{
    int idx = 0;
    for (HelpTopic* hl : m_helper.m_topics.m_topics) {

        QListWidgetItem* item= new QListWidgetItem();
        item->setData(Qt::UserRole,idx);
        item->setText(hl->m_text);
        ui->lstTopic->addItem(item);
        idx++;
    }
}

void DialogHelp::FillItems(int curidx)
{
    int idx = 0;
    ui->lstItems->clear();
    for (HelpTopic* hl : m_helper.m_topics.m_topics[curidx]->m_topics) {

        QListWidgetItem* item= new QListWidgetItem();
        item->setData(Qt::UserRole,idx);
        item->setText(hl->m_text);
        ui->lstItems->addItem(item);
        idx++;
    }

}

void DialogHelp::FillHelpText()
{
    if (m_curItem==-1)
        m_curItem=0;
    if (m_curTopic>=m_helper.m_topics.m_topics.count())
        return;
    if (m_curItem>=m_helper.m_topics.m_topics[m_curTopic]->m_topics.count())
        return;


    QString s = m_helper.m_topics.m_topics[m_curTopic]->m_topics[m_curItem]->m_info;
    ui->txtHelp->setText(s);
}

void DialogHelp::on_pushButton_clicked()
{
    close();
}



void Helper::LoadFromResource()
{
    QFile f(":resources/text/Documentation.txt");
    f.open(QIODevice::ReadOnly | QFile::Text) ;
    QString txt = f.readAll();
    f.close();
    m_lst = txt.split("\n");
    m_curIdx = 0;

//    Build(m_topics,0);
    HelpTopic* zero;
    HelpTopic* one;
    for (int i=0;i<m_lst.count();i++) {
        int tab = 0;
        QString s=m_lst[i].replace("    ", "\t");
        if (s.trimmed()=="")continue;
        while (s[tab]=='\t') tab++; // Find number of tabs
        if (tab==0) {
            zero = new HelpTopic();
            zero->m_text = s.replace("\t", "");
            m_topics.m_topics.append(zero);
       }
        if (tab==1) {
            one = new HelpTopic();
            one->m_text = s.replace("\t", "");
            zero->m_topics.append(one);
       }
        if (tab==2) {
            s=s.replace("\t", "").replace("@code", "#50a0C0");
            one->m_info = one->m_info + s+"\n";
       }


    }

}

void Helper::Build(HelpTopic& t, int curTab)
{

    QString s = m_lst[m_curIdx].replace("    ","\t");
    int noTabs = 0;
    while (s[noTabs]=='\t') noTabs++; // Find number of tabs

    while (curTab==noTabs && m_curIdx<m_lst.count()) {
        s = m_lst[m_curIdx].replace("    ","\t");
        noTabs = 0;
        //qDebug() <<"First: " <<s[0];
        while (s[noTabs]=='\t') noTabs++; // Find number of tabs
        s=s.replace("\t","");
        qDebug() << s << " c,n : "<< curTab << "," << noTabs;
        m_curIdx++;
        if (curTab<=1 && curTab == noTabs) {
            HelpTopic n;
            n.m_text = s;
            //m_curIdx++;

            //qDebug() << "cur: "<<curTab << ", " << noTabs << " : "<<n.m_text;
            //t.m_topics.append(n);
            Build(n, curTab+1);
            qDebug() << "Item: " << n.m_text;
        }
        else {
            qDebug() << "Adding text:" <<s;
            t.m_text=t.m_text+s+"\n";

        }
        noTabs = 0;

    }
}

void DialogHelp::on_lstTopic_itemClicked(QListWidgetItem *item)
{
    m_curTopic = item->data(Qt::UserRole).toInt();
    FillItems(m_curTopic);
}

void DialogHelp::on_lstItems_itemClicked(QListWidgetItem *item)
{
    m_curItem = item->data(Qt::UserRole).toInt();
    FillHelpText();
}

void DialogHelp::on_leSearch_textChanged(const QString &arg1)
{
    for (int i=0;i<m_helper.m_topics.m_topics.count();i++) {
        for (int j=0;j<m_helper.m_topics.m_topics[i]->m_topics.count();j++) {
            QString lbl = m_helper.m_topics.m_topics[i]->m_topics[j]->m_text;
            QString str = m_helper.m_topics.m_topics[i]->m_topics[j]->m_info;
            if ( lbl.toLower().contains(arg1.toLower())) { // ||  str.toLower().contains(arg1.toLower())) {
                m_curTopic = i;
                m_curItem = j;
                FillItems(m_curTopic);
                FillHelpText();
                m_curTopic = i;
                m_curItem = j;
                ui->lstTopic->setCurrentRow (m_curTopic);
                ui->lstItems->setCurrentRow(m_curItem);
            }
        }

    }
}

void DialogHelp::on_lstItems_currentRowChanged(int currentRow)
{
    m_curItem = currentRow;
    FillHelpText();


}
