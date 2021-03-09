#include "widgetpattern.h"
#include "ui_widgetpattern.h"

WidgetPattern::WidgetPattern(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetPattern)
{
    ui->setupUi(this);
}

WidgetPattern::~WidgetPattern()
{
    delete ui;
}

QComboBox *WidgetPattern::getPatternCmb()
{
    return ui->cbmPattern;
}

void WidgetPattern::SetData(TRSEDocument* doc, CIniFile* colors, QSharedPointer<DataStorage> ds) {
    ui->Pattern->m_doc = doc;
    ui->Pattern->m_colors = colors;
    ui->Pattern->setData(ds);
    update();
    ui->Pattern->update();
    ui->Pattern->viewport()->update();
}

QByteArray &WidgetPattern::getData()
{
    return ui->Pattern->m_pdata->m_data;
}

void WidgetPattern::on_cbmPattern_currentIndexChanged(int index)
{
    emit emitReloadPatterns();
}
