#include "interpreter.h"
#include <QDebug>
Interpreter::Interpreter(Parser* p)
{
    m_parser = p;
}

void Interpreter::Parse()
{
    m_tree = nullptr;
    qDebug() << "Parsing..";
    try {

        m_tree = m_parser->Parse();
    } catch (FatalErrorException e) {
        qDebug() << "ERROR parse " << e.message;
        HandleError(e, "Error during parsing:");
    }

}

void Interpreter::Visit(Node* n)
{
    n->Execute(nullptr, 0);
}

void Interpreter::Interpret()
{
    if (m_tree!=nullptr)
        try {
        Visit(m_tree);
    } catch (FatalErrorException e) {

    }

}

bool Interpreter::Build(Interpreter::Type type, QString project_dir, QStringList zeropages)
{
    if (m_tree==nullptr) {
        qDebug() << "Interpreter::Build : tree not parsed!";
        return false;
    }
    if (m_assembler)
        delete m_assembler;

    if (type==MOS6502)
        m_assembler = new AsmMOS6502();
    if (type==PASCAL)
        m_assembler = new AsmPascal();


    m_assembler->InitZeroPointers(zeropages);

    m_assembler->m_projectDir = project_dir;

    if (m_tree!=nullptr)
        try {
            m_tree->Build(m_assembler);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during build");
            return false;
         }
 /*       try {
           // m_tree->ExecuteSym(m_assembler->m_symTab);
        } catch (FatalErrorException e) {
            HandleError(e,"Error during symbolic check");
            return false;
    }*/

    for (MemoryBlock& mb:m_parser->m_userBlocks)
        m_assembler->blocks.append(mb);

    m_assembler->Label("EndSymbol");
    m_assembler->Connect();
    m_assembler->Optimise();
    CleanupCycleLinenumbers();
    CleanupBlockLinenumbers();
    return true;

}

void Interpreter::CleanupCycleLinenumbers()
{

    QMap<int, int> cycles;

    for (int i: m_assembler->m_cycles.keys()) {

        int count = m_assembler->m_cycles[i];
        int nl = i;
        for (FilePart& fp : m_parser->m_lexer->m_includeFiles) {
            // Modify bi filepart
            if (nl>fp.m_startLine && nl<fp.m_endLine) {
                cycles[fp.m_startLine]+=count;
                count=0;
            }

            if (nl>=fp.m_endLine)
                nl-=fp.m_count-1;
        }
        if (count!=0)
            cycles[nl] = count;


    }

    m_assembler->m_cycles = cycles;
}

void Interpreter::CleanupBlockLinenumbers()
{
    QMap<int, int> blocks;

    for (int i: m_assembler->m_blockIndent.keys()) {

        int count = m_assembler->m_blockIndent[i];
        int nl = i;
        for (FilePart& fp : m_parser->m_lexer->m_includeFiles) {
            // Modify bi filepart
            if (nl>fp.m_startLine && nl<fp.m_endLine) {
                blocks[fp.m_startLine]+=count;
                count=0;
            }

            if (nl>=fp.m_endLine)
                nl-=fp.m_count-1;
        }
        if (count!=0)
            blocks[nl] = count;


    }

    m_assembler->m_blockIndent = blocks;

}

void Interpreter::SaveBuild(QString filename)
{
    if (!m_assembler)
        return;
    m_assembler->Save(filename);
}

void Interpreter::HandleError(FatalErrorException fe, QString e)
{
    QString msg = "";
    int linenr = fe.linenr;
    QString file = "";
    FindLineNumberAndFile(fe.linenr, file, linenr);
    //linenr = fe.linenr;


    QString line = "on line: " + QString::number(linenr+1);
    if (file!="")
        msg+="In file : " + file + "\n";

    fe.file=file;

    msg +="\nFatal error " + line;
    if (linenr<m_parser->m_lexer->m_lines.count() && linenr>=0)
        msg+="\nSource: " + m_parser->m_lexer->m_lines[linenr];
    msg+="\n\nMessage: ";
    Pmm::Data::d.lineNumber = linenr+1;

    recentError = fe;
    ErrorHandler::e.CatchError(fe, e + msg);

}

void Interpreter::FindLineNumberAndFile(int inLe, QString& file, int& outle)
{
    file="";
    outle = inLe;
    if (m_parser->m_lexer->m_includeFiles.count()==0) {
        return;
    }

    int cur = inLe;

    qDebug() << "input line number: " << inLe;
    qDebug() << "Start line: " << m_parser->m_lexer->m_includeFiles[0].m_startLine;
    if (cur<=m_parser->m_lexer->m_includeFiles[0].m_startLine) {
        return;
    }


    for (FilePart fp: m_parser->m_lexer->m_includeFiles) {
        if (inLe >= fp.m_startLine && inLe<fp.m_endLine) {
            file = fp.m_name;
            outle = inLe - fp.m_startLine;
            return;
        }
        qDebug() << "Include file size : " << (fp.m_endLine-fp.m_startLine);
        cur=cur - (fp.m_endLine-fp.m_startLine);
    }
    outle = cur;
}
