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
    QSharedPointer<CIniFile> m_project, m_settings;
    QMap<QString,QString> m_vals;
    QMap<QString,QStringList> m_options;
    SourceBuilder* m_builder = nullptr;
    QString m_outputFile = "";
    QApplication* app = nullptr;
    bool m_hasError = false;
    int m_failure = 0;
    //QCoreApplication* core = nullptr;
    ClascExec(int argc, char *argv[]);

    void RequireParam(QString param);
    void RequireFile(QString param);

    int Perform();
    int CompileFromProject(QString sourceFile, bool assemble);
    int Assemble(QString file);

    void PrintUsage();

    void Out(QString m);

/*    QTextStream& out()
    {
        static QTextStream ts( stdout );
        return ts;
 //       return qDebug;
    }*/
};



#endif // clasc_H
