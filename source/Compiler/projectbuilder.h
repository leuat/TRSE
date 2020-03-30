#ifndef PROJECTBUILDER_H
#define PROJECTBUILDER_H

#include "source/Compiler/sourcebuilder.h"

#include <QThread>
#include <QString>

class ProjectBuilder : public QThread
{
    Q_OBJECT

public:
    ProjectBuilder();
    ProjectBuilder(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> project, QString curDir);


    QSharedPointer<CIniFile> m_projectIniFile, m_iniFile;
    QString m_curDir;
    QStringList m_list;

    QString m_output;

    void Message(QString s, bool term=true);

    void Build(QString file);

    void run() override;

    void LoadBuildList(QStringList buildList);

signals:
    void emitTextUpdate();

};

#endif // PROJECTBUILDER_H
