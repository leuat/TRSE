#ifndef POSTOPTIMISER_H
#define POSTOPTIMISER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QMap>
#include <QVector>


class SourceLine {
public:
    int m_lineNumber;
    QString m_orgLine, m_cmd;
    bool m_isLabel = false;
    bool m_remove = false;
    bool m_potentialOptimise = false;
    bool m_forceOptimise = false;

    QMap<QString, QString> m_changeRegs;
    SourceLine() = default;
    SourceLine(int lnr, QString line) {
        m_lineNumber = lnr;
        m_orgLine = line;
    }
};

class PostOptimiser : public QObject
{
    Q_OBJECT
public:
    QVector<SourceLine> m_lines;
    PostOptimiser();
    int m_noPasses = 1;
    int m_linesOptimized = 0;
    QMap<QString, QString> m_regs;
    QStringList m_registers;
    QStringList m_branches;
    QStringList m_registerChangingCommands;
    QStringList m_bops;

    QStringList PostOptimize(QStringList& src);
    void CreateLines(QStringList& src);

    virtual void Analyze(SourceLine& line) = 0;
    virtual void InitRegisters() = 0;
    virtual void Passes();
    void PassRegisterChanges();
    void RemoveAllFlagged();

};

#endif // POSTOPTIMISER_H
