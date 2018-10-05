#include "assembler.h"

//QMap<QString, bool> LabelStack::sNumbersUsed;

int Appendix::s_id = 0;


Assembler::Assembler()
{
  //  LabelStack::sNumbersUsed.clear();
    m_symTab = new SymbolTable();
}

void Assembler::Save(QString filename) {
    if (QFile::exists(filename))
        QFile::remove(filename);

    QFile file(filename);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        for (QString& s: m_source)
            stream<< s<< "\n";
    }
    file.close();
}

void Assembler::Nl()
{
    m_source << "";
}

void Assembler::Write(QString str, int level)
{
    if (m_currentBlock==nullptr) {

        QString s ="";
        for (int i=0;i<level;i++)
            s+="\t";
        s+=str;
        m_source.append(s);
    }
    else {
        m_currentBlock->Append(str,level);
//        qDebug() << "CURRENTBLOCK: " << str;
    }


    int cnt = CountCycles(str);
    for (int i=0;i<m_cycleCounter.count();i++)
        m_cycleCounter[i] += cnt;

}

bool caseInsensitiveLessThan(const Appendix &s1, const Appendix &s2)
{
    QString sa = s1.m_pos.toLower().replace("$","0x");
    QString sb = s2.m_pos.toLower().replace("$","0x");

    int a,b;
    bool ok;

//    qDebug() << sa << " vs " << sb;

    if (sa.contains("0x")) a=sa.toInt(&ok, 16);
    else
        a = sa.toInt(&ok, 10);

    if (sb.contains("0x")) b=sb.toInt(&ok, 16);
    else
        b = sb.toInt(&ok, 10);


    return a < b;
}

/*int doSomething()
{
    QStringList list;
    list << "AlPha" << "beTA" << "gamma" << "DELTA";
    qStableSort(list.begin(), list.end(), caseInsensitiveLessThan);
    // list: [ "AlPha", "beTA", "DELTA", "gamma" ]
}*/
void Assembler::SortAppendix()
{
 /*   for (int i=0;i<m_appendix.count();i++)
        qDebug() << "appos:" << m_appendix[i].m_pos;*/
   qSort(m_appendix.begin(), m_appendix.end(), caseInsensitiveLessThan);
}

void Assembler::PushCounter()
{
    m_cycleCounter.append(0);
}

void Assembler::PopCounter(int ln)
{
    int i = m_cycleCounter.last();
    m_cycleCounter.removeLast();
    if (i>m_cycles[ln]) // Only count largest number
       m_cycles[ln] = i;
    //   return i;
}

void Assembler::PushBlock(int ln)
{
    if (m_blockIndent.contains(ln))
        //PushBlock(ln+1);
        m_blockIndent[ln] = m_blockIndent[ln]+1;
    else
        m_blockIndent[ln] = 1;
    //   qDebug()<< "push: " <<m_blockCounter.count()  << " at " << ln;

}

void Assembler::PopBlock(int ln)
{
    if (m_blockIndent.contains(ln))
//        PopBlock(ln-1);
        m_blockIndent[ln] = m_blockIndent[ln]-1;

    else
        m_blockIndent[ln] = -1;

}

int Assembler::CountCycles(QString s)
{
    QStringList lst = s.split("\n");
    int n=0;
    for (QString s : lst) {
//        s=s.remove("\t");
        s=s.trimmed();
        if (s=="") continue;
        if (s.startsWith(";")) continue;
        int i = s.indexOf(";");
        if (i!=-1) { // Remove all comments
            s = s.remove(i, s.length());
        }
        //if (s[0]!="\t") continue; // is label
        QStringList l = s.split(" ");
        if (l[0].count()==3) {
            //qDebug() << "Adding :" << s;
            int cnt = CountInstructionCycle(l);
            n+=cnt;
//            if (cnt!=0) {
  //              qDebug() << "Counted for : '" <<s <<"' : " <<cnt;
    //        }
        }

    }
    return n;

}


void Assembler::Asm(QString s, QString comment)
{
    QString c = "\t;" + comment;
    if (comment=="") c="";
    Write(s+c ,1);
}

void Assembler::Label(QString s)
{
    Write(s,0);
}

void Assembler::Term(QString s, bool write)
{
    m_term+=s;
    if (write) {
        Asm(m_term);
        ClearTerm();

    }
}

void Assembler::Term()
{
    Asm(m_term);
    ClearTerm();
}

void Assembler::Connect()
{
    // Connect with temp vars
    QStringList newSource;
    for (int i=0;i<m_varDeclEndsLineNumber;i++) {
        newSource<<m_source[i];
    }
    newSource << " ; Temp vars section";
    newSource<< m_tempVars;
    newSource << " ; Temp vars section ends";
    for (int i=m_varDeclEndsLineNumber;i<m_source.count(); i++) {
        newSource<<m_source[i];
    }
    m_source = newSource;

    //m_source<<m_appendix;
//    m_appendix.append(m_extraBlocks);
    SortAppendix();
  //  qDebug() << m_appendix[0].m_source;
    for (int i=0;i<m_appendix.count();i++) {
//        qDebug() << m_appendix[i].m_source;
        m_source << m_appendix[i].m_source;
    }
    m_appendix.clear();
}



QString Stack::current() const
{
    return m_current;
}

