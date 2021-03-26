#include "widgetpattern.h"
#include "ui_widgetpattern.h"

WidgetPattern::WidgetPattern(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPattern)
{
    ui->setupUi(this);
    connect(ui->Pattern,SIGNAL(emitChangeTriggered()),this,SLOT(PropagateUpdate()));
    connect(ui->Pattern,SIGNAL(emitMove(int,int)),this,SLOT(acceptMove(int,int)));
    connect(ui->Pattern,SIGNAL(emitSound(QByteArray)),this,SLOT(acceptSound(QByteArray)));
}

WidgetPattern::~WidgetPattern()
{
    delete ui;
}

QComboBox *WidgetPattern::getPatternCmb()
{
    return ui->cbmPattern;
}


void WidgetPattern::SetData(TRSEDocument* doc, CIniFile* colors, QSharedPointer<DataStorage> ds, int curPatt, int curColumn) {
    ui->Pattern->m_doc = doc;
    ui->Pattern->m_colors = colors;
    ui->Pattern->setData(ds);
    update();
    RefreshAll();
    m_curPattern = curPatt;
    m_curColumn = curColumn;
}

void WidgetPattern::UpdateData(QByteArray& ba)
{
    ui->Pattern->m_pdata->m_data = ba;
}

QByteArray &WidgetPattern::getData()
{
    return ui->Pattern->m_pdata->m_data;
}

void WidgetPattern::RefreshAll()
{
    update();
    ui->Pattern->update();
    ui->Pattern->viewport()->update();
    ui->Pattern->repaint();

}

void WidgetPattern::SetCursorPosition(int pos)
{
       ui->Pattern->setCursorPos(pos);
       ui->Pattern->resetSelection(pos);
//       viewport() -> update();
//        ui->Pattern->viewport()->update();

       ui->Pattern->setFocus();
       RefreshAll();
}

void WidgetPattern::SetCurrentRow(int row) {
    ui->Pattern->m_curPlayerLine = row;
}

void WidgetPattern::on_cbmPattern_currentIndexChanged(int index)
{
    emit emitReloadPatterns();
}

void WidgetPattern::PropagateUpdate()
{
    emit emitUpdatePatterns(this,m_curPattern);
}

void WidgetPattern::acceptMove(int direction, int pos)
{
    emit emitMove(direction,pos,m_curColumn);
}
