#include "formfjong.h"
#include "ui_formfjong.h"

FormFjong::FormFjong(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormFjong)
{
    ui->setupUi(this);

    m_font.setFamily("Courier");
    m_font.setFixedPitch(true);
    m_font.setPointSize(14);//m_iniFile->getdouble("font_size"));



    ui->txtEditor->setFont(m_font);
}

FormFjong::~FormFjong()
{
    delete ui;
}

void FormFjong::SetText(QString txt)
{
    ui->txtEditor->setPlainText(txt);
//    ui->txtEditor->m_textChanged = false;
}

void FormFjong::Save(QString filename)
{
    if (QFile::exists(filename))
        QFile::remove(filename);
    QString txt = ui->txtEditor->document()->toPlainText();
    QFile file(filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate))
    {
        QTextStream stream(&file);
        stream << txt;
    }
    file.close();
    m_iniFile->Save();
//    ui->txtEditor->m_textChanged = false;
}

void FormFjong::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        SetText(file.readAll());
    }
    file.close();
    UpdateFromIni();
}

void FormFjong::UpdateFromIni()
{
    QFontMetrics metrics(m_font);

    m_font.setPointSize(m_iniFile->getdouble("font_size"));
    ui->txtEditor->setFont(m_font);
    ui->txtEditor->setTabStopWidth(m_iniFile->getInt("tab_width") * metrics.width(' '));


}

void FormFjong::Init(LuaScript &s)
{
    lua_getglobal(s.L, "init");
    /*        lua_pushnumber(s.L, x);
        lua_pushnumber(s.L, y);
  */
    lua_pcall(s.L, 0, 0, 0);

    //int result = (int)lua_tonumber(L, -1);
    lua_pop(s.L, 1);
    //        return result;
}


void FormFjong::Run()
{
    SaveCurrent();
    m_currentSourceFile.replace("//","/");

    DialogEffects* de = new DialogEffects(m_currentSourceFile);
    de->Init();
    de->exec();
    delete de;

/*    LuaScript script(m_currentSourceFile);

    lua_register(script.L, "Test", Test);

    luaL_dostring(script.L, "Init()");
*/



}
