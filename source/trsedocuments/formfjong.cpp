#include "formfjong.h"
#include "ui_formfjong.h"

FormFjong::FormFjong(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormFjong)
{
    ui->setupUi(this);

    m_type = FJONG;

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
    ui->txtEditor->m_textChanged = false;
    m_documentIsChanged = false;
}

bool FormFjong::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        SetText(file.readAll());
    }
    file.close();
    UpdateFromIni();
    SetupHighlighter();
    ui->txtEditor->m_textChanged = false;
    m_documentIsChanged = false;
    return true;
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
void FormFjong::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pro)
{
    TRSEDocument::InitDocument(t,ini, pro);
    QString fnt = m_iniFile->getString("editor_font");
    if (fnt=="") fnt ="Courier";
    m_font.setFamily(fnt);
    m_font.setFixedPitch(true);
    m_font.setPointSize(m_iniFile->getdouble("font_size"));


}



void FormFjong::Run()
{
    SaveCurrent();
    m_currentSourceFile.replace("//","/");

    DialogEffects* de = new DialogEffects(m_currentSourceFile);
    de->Init(m_currentDir);
    de->exec();
    delete de;

/*    LuaScript script(m_currentSourceFile);

    lua_register(script.L, "Test", Test);

    luaL_dostring(script.L, "Init()");
*/



}

void FormFjong::SetupHighlighter()
{
    if (highlighter != nullptr)
        delete highlighter;
    CIniFile colors;

    colors.Load(Util::GetSystemPrefix() + "themes/" + m_iniFile->getString("theme_fjong"));
    ui->txtEditor->InitColors(colors);

    QPalette p = ui->txtEditor->palette();
    p.setColor(QPalette::Base, colors.getColor("backgroundcolor"));
    p.setColor(QPalette::Text, colors.getColor("textcolor"));
    ui->txtEditor->setPalette(p);
    highlighter = new FjongHighlighter(colors, 0, ui->txtEditor->document());

    //    qDebug() << "UPDATE " << m_iniFile->getString("theme");

}


void FormFjong::SearchInSource()
{
    m_currentFromPos = ui->txtEditor->document()->toPlainText().toLower().indexOf(ui->leSearch->text().toLower(), m_searchFromPos);
    QTextCursor cursor(ui->txtEditor->document()->findBlock(m_currentFromPos));
    ui->txtEditor->setTextCursor(cursor);
}

void FormFjong::keyPressEvent(QKeyEvent *e)
{
    TRSEDocument::keyPressEvent(e);
    if (e->key() == Qt::Key_Escape && ui->leSearch->hasFocus()) {
        ui->txtEditor->setFocus();
    }
//    qDebug() << ui->txtEditor->m_textChanged    ;
/*    if (e->key()==Qt::Key_W && (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        m_documentIsChanged  = ui->txtEditor->m_textChanged;
        emit requestCloseWindow();
    //    Data::data.requestCloseWindow = true;

//    if (ui->txtEditor->m_textChanged)
    }
*/
    if (e->key()==Qt::Key_J && (QApplication::keyboardModifiers() & Qt::ControlModifier)) AutoFormat();
    if (e->key()==Qt::Key_F && QApplication::keyboardModifiers() & Qt::ControlModifier) {
        ui->leSearch->setText("");
        m_searchFromPos = ui->txtEditor->textCursor().position();
        ui->leSearch->setFocus();
    }

  /*  if (e->key()==Qt::Key_F1) {
        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();

        DialogHelp* dh = new DialogHelp(this, word, m_defaultPalette);
//        dh->setPalette(m_defaultPalette);
     //   QApplication::setPalette(m_defaultPalette);

        dh->show();

    }
*/

    if (e->key()==Qt::Key_F1) {
        QTextCursor tc = ui->txtEditor->textCursor();
        tc.select(QTextCursor::WordUnderCursor);
        QString word = tc.selectedText();

        DialogHelp* dh = new DialogHelp(nullptr, word, m_defaultPalette);
//        dh->setPalette(m_defaultPalette);
     //   QApplication::setPalette(m_defaultPalette);

        dh->show();

    }


    if (e->key() == Qt::Key_R &&  (QApplication::keyboardModifiers() & Qt::ControlModifier)) {
        Build();
  //      Run();
    }



}


void FormFjong::on_leSearch_textChanged(const QString &arg1)
{
    SearchInSource();

}
void FormFjong::on_leSearch_returnPressed()
{
    m_searchFromPos=m_currentFromPos+1;
    SearchInSource();

}
