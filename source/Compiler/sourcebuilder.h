#ifndef SOURCEBUILDER_H
#define SOURCEBUILDER_H

#include <QString>
#include "source/Compiler/compilers/factorycompiler.h"
#include "source/Compiler/systems/abstractsystem.h"
#include "source/Compiler/systems/factorysystem.h"
#include <QThread>
#include <QSharedPointer>


class SourceBuilder : public QObject
{
    Q_OBJECT
public:
    SourceBuilder();
    SourceBuilder(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> project, QString curDir, QString curSourceFile);
    ~SourceBuilder();

    QSharedPointer<CIniFile> m_iniFile, m_projectIniFile;

    QMap<QString,QStringList> m_options;

    bool m_buildSuccess = false;
    bool m_assembleSuccess = false;
    bool m_useSyntaxSystem = false;
    bool m_isTRU = false;
    bool m_isShadow = false;

    QSharedPointer<AbstractSystem> m_system;
    QSharedPointer<Compiler> compiler;
    //QSharedPointer<Compiler> compiler;

//    AbstractSystem* m_system = nullptr;
    //Compiler* compiler = nullptr;
    bool Build(QString source);
    void Destroy();

    bool Assemble();

    QString m_buildString = "";

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
public:
signals:
    void EmitBuildString();
    void emitRequestSystemChange(QString);
public slots:
    void AcceptParserTick(QString val);
    void AcceptRequestSystemChange(QString val);

};

#endif // SOURCEBUILDER_H
