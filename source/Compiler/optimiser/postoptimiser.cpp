#include "postoptimiser.h"
#include <QDebug>
PostOptimiser::PostOptimiser()
{

}



QStringList PostOptimiser::PostOptimize(QStringList& src) {
    QStringList ret = src;
    m_linesOptimized = 0;

    for (int i=0;i<m_noPasses;i++) {
        CreateLines(ret);
        Passes();
        ret.clear();
        for (SourceLine& l:m_lines)
            ret<<l.m_orgLine;

    }


    return ret;
}

void PostOptimiser::CreateLines(QStringList& lst)
{
    int ln = 0;
    InitRegisters();
    m_lines.clear();
    for (QString& s:lst) {
        m_lines.append(SourceLine(ln,s));
        Analyze(m_lines.last());
//        qDebug() << s;
        ln++;

    }

}

void PostOptimiser::Passes()
{
    PassRegisterChanges();
}

void PostOptimiser::PassRegisterChanges()
{
    m_regs.clear();
    for (int i=0;i<m_lines.count()-1;i++) {
        SourceLine& cur = m_lines[i];

        if (m_registerChangingCommands.contains(cur.m_cmd)) {
            m_regs.clear();
//            qDebug() << "Clearing register changing: " << cur.m_orgLine <<cur.m_cmd;

        }

        if (cur.m_isLabel) {
            m_regs.clear(); // Clear registers on label!
  //          qDebug() << "Clearing labels: " << cur.m_orgLine;
        }



        if (cur.m_potentialOptimise || cur.m_forceOptimise) {
            bool allequals = true;
    //        qDebug() << "Testing: " <<cur.m_orgLine;
            for (QString s: cur.m_changeRegs.keys()) {
//                qDebug() << "CONTAINS " <<s << cur.m_changeRegs[s] <<m_regs.contains(s);
                if (m_regs.contains(s)) {
  //                  qDebug() << "   Comparing : "<<s<<m_regs[s]<<cur.m_changeRegs[s];
                    if (m_regs[s] != cur.m_changeRegs[s])
                        allequals = false;
                    if (m_regs[s]=="")
                        allequals = false;
                }
                else allequals = false;
            }
            if (allequals || cur.m_forceOptimise) {
    //            qDebug() << "REMOVING " <<cur.m_orgLine;
                cur.m_remove = true;
            }
        }
        if (!cur.m_remove)
        {
            for (QString s: cur.m_changeRegs.keys())
                m_regs[s] = cur.m_changeRegs[s];
        }
    }

    RemoveAllFlagged();

}

void PostOptimiser::RemoveAllFlagged()
{
    QVector<SourceLine> nlst;
    for (auto& l:m_lines)
        if (!l.m_remove)
            nlst.append(l);
        else {

            if (!l.m_orgLine.contains(";REMOVAL") && (!l.m_orgLine.contains(";keep"))) {
                l.m_orgLine+=";REMOVAL";
  //              qDebug() << "Removing : "<<l.m_lineNumber << l.m_orgLine;
            }
//            nlst.append(l);

            m_linesOptimized++;
        }
    m_lines = nlst;
}
