#include "dialogexport3d.h"
#include "ui_dialogexport3d.h"
#include <QFileDialog>
DialogExport3D::DialogExport3D(QSharedPointer<CIniFile> ini, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExport3D)
{
    ui->setupUi(this);
    m_ini = ini;
    FillFromIni();

}

DialogExport3D::~DialogExport3D()
{
    delete ui;
}

void DialogExport3D::on_pushButton_clicked()
{
    FillToIni();
    close();

}

void DialogExport3D::on_pushButton_2_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("C64 Emulator location"), "", "*.obj");
    if (filename!="")
       ui->leInput->setText(filename);

}

void DialogExport3D::on_pushButton_3_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("C64 Emulator location"), "", "*");
    if (filename!="")
       ui->leOutputDir->setText(filename);

}

void DialogExport3D::on_pushButton_4_clicked()
{
    ObjLoader obj;
    QString input = ui->leInput->text();
    QString outputDir = ui->leOutputDir->text();
    QString output = ui->leOutput->text();

    QString base = outputDir + QDir::separator() + output;

    ui->textBrowser->clear();

    if (!QFile::exists(input)) {
        Message("Could not open file : " + input);
        return;
    }

    obj.Load(input);
    try {
        QStringList vec = ui->leVecDisp->text().split(",");
        QVector3D shift = QVector3D(vec[0].toFloat(),vec[1].toFloat(),vec[2].toFloat());
        float scale = ui->leSize->text().toFloat();
        float minLineLength = ui->leMinLineLength->text().toFloat();

        obj.Parse();

        if (ui->comboBox->currentText()=="Amiga") {

            Message(obj.ExportAmigaVerts(base+"_verts.bin",scale,shift*-1));
            Message(obj.ExportAmigaFaces(base+"_faces.bin"));
            Message(obj.ExportAmigaLinesFromFaces(base+"_lines.bin",minLineLength));
            Message(obj.ExportAmigaNormalsLines(base+"_line_normals.bin",64.0f));
            Message(obj.ExportAmigaFaceNormals(base+"_face_normals.bin",64.0f));
        //Message(obj.ExportAmigaFaceNormals(base+"_line_normals.bin",64.0f));
        }
        if (ui->comboBox->currentText()=="Lua") {
            Message(obj.ExportLua(base+".lua",output,scale,shift*-1));

        }

        Message("Conversion OK.\n");
    } catch (QString s) {
        Message(s);
    }

}

void DialogExport3D::Message(QString txt)
{
    QString text = ui->textBrowser->toPlainText();
    text = text+"\n"+txt;
    ui->textBrowser->setText(text);
}

void DialogExport3D::FillFromIni()
{
    ui->leOutput->setText(m_ini->getString("export3d_last_output"));
    ui->leOutputDir->setText(m_ini->getString("export3d_last_outputdir"));
    ui->leSize->setText(QString::number(m_ini->getdouble("export3d_last_size")));
    ui->leVecDisp->setText(Util::toString(m_ini->getStringList("export3d_last_shift")));
    ui->leInput->setText(m_ini->getString("export3d_last_input"));

    ui->leMinLineLength->setText(QString::number(m_ini->getdouble("export3d_last_minlinelength")));

}


void DialogExport3D::FillToIni()
{
    m_ini->setString("export3d_last_output",ui->leOutput->text());
    m_ini->setString("export3d_last_outputdir",ui->leOutputDir->text());
    m_ini->setString("export3d_last_input",ui->leInput->text());
    m_ini->setFloat("export3d_last_size",ui->leSize->text().toFloat());
    m_ini->setStringList("export3d_last_shift",ui->leVecDisp->text().split(","));

    m_ini->setFloat("export3d_last_minlinelength",ui->leMinLineLength->text().toFloat());

}
