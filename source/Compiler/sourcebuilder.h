#ifndef SOURCEBUILDER_H
#define SOURCEBUILDER_H

#include <QString>
#include "source/Compiler/compilers/factorycompiler.h"
#include "source/Compiler/systems/abstractsystem.h"
#include "source/Compiler/systems/factorysystem.h"
#include <QThread>


class SourceBuilder
{
public:
    SourceBuilder();
    SourceBuilder(CIniFile* ini, CIniFile* project, QString curDir, QString curSourceFile);
    ~SourceBuilder();

    CIniFile m_iniFile, m_projectIniFile;

    bool m_buildSuccess = false;
    bool m_assembleSuccess = false;
    bool m_useSyntaxSystem = false;


    AbstractSystem* m_system = nullptr;
    Compiler* compiler = nullptr;
    bool Build(QString source);


    bool Assemble();

    QString getOutput();
    void AddMessage(QString msg) {
        m_output+=msg;
    }
    QString m_currentSourceFile="";
    QString m_curDir = "";
    QString m_filename="";

private:

    QString m_source = "";
    QString m_output = "";


    QStringList getFileList();

    void BuildSuccesString();


};

#endif // SOURCEBUILDER_H
