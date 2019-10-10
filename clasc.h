#ifndef clasc_H
#define clasc_H

#include <QCoreApplication>

#include <QMap>
#include <QString>
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/sourcebuilder.h"
#include "source/OrgAsm/orgasm.h"

class ClascExec {
public:
    QStringList m_args;
    CIniFile m_project, m_settings;
    QMap<QString,QString> m_vals;
    SourceBuilder* m_builder = nullptr;
    QString m_outputFile = "";
    bool m_hasError = false;
    int m_failure = 0;
    ClascExec(int argc, char *argv[]);

    void RequireParam(QString param);
    void RequireFile(QString param);

    int Perform();
    int CompileFromProject(QString sourceFile);
    int Assemble(QString file);

    void PrintUsage();

    QTextStream& out()
    {
        static QTextStream ts( stdout );
        return ts;
    }
};



#endif // clasc_H
