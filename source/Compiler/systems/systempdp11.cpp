#include "systempdp11.h"
#include "source/messages.h"

SystemPDP11::SystemPDP11(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" << "chipmem" <<"aligned" << "compressed";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global" << "aligned";
    m_processor = PDP11;
    m_allowedBaseTypes<<"LONG";
    m_allowClasses = true;
    m_supportsInclusiveFor = false;
}

void SystemPDP11::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;



    //qDebug() << m_settingsIni->getString("assembler");
    QProcess process;
    QStringList params;
    // -kick1hunks  -o example$1 -nosym source$1.asm
    params << "-m" <<"pdpy11" << filename+".asm";
    params << "--lst" <<"-o" << filename+".bin";
    // qDebug() << params;
    QString python = "python";
//    python = "/usr/bin/python3";
    if (m_settingsIni->getdouble("use_python")==0)
        python = "python3";

    auto ue = QProcessEnvironment::systemEnvironment();
    process.setWorkingDirectory(currentDir);
    process.setProcessEnvironment(ue);
    process.start(python, params);
    process.waitForFinished();
  /*  qDebug() << python << params;
    output = process.readAllStandardOutput();
    qDebug() << output;
    output += process.readAllStandardError();
    qDebug() << output;
*/
    if (output.contains("No module named pdpy11")) {
        text  += "<br><font color=\"#FF6040\">Please install python and pdpy11, ie pip3 install pdpy11 before compiling.</font>";
        m_buildSuccess = false;
        return;
    }

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //    qDebug() << "*********" << output;

    text+=output;
}
