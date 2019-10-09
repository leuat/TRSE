#ifndef CRAS_H
#define CRAS_H

#include <QCoreApplication>

#include <QMap>
#include <QString>
#include "source/LeLib/util/cinifile.h"
#include "source/Compiler/sourcebuilder.h"

class CRasExec {
public:
    QStringList m_args;
    CIniFile m_project, m_settings;
    QMap<QString,QString> m_vals;
    SourceBuilder* m_builder = nullptr;
    bool m_hasError = false;
    CRasExec(int argc, char *argv[]);

    void RequireParam(QString param);
    void RequireFile(QString param);

    int Perform();
    int CompileFromProject(QString sourceFile);

    QTextStream& out()
    {
        static QTextStream ts( stdout );
        return ts;
    }
};



#endif // CRAS_H
