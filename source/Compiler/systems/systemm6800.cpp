#include "systemm6800.h"
#include "source/messages.h"

void SystemM68000::Assemble(QString &text, QString filename, QString currentDir)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    if (!QFile::exists(m_settingsIni->getString("vasmm")))
        Messages::messages.DisplayMessage(Messages::messages.NO_VASMM);


    //qDebug() << m_settingsIni->getString("assembler");
        QProcess process;
        QStringList params;
       // -kick1hunks  -o example$1 -nosym source$1.asm
        params << "-kick1hunks";
        params << "-Fhunkexe";
        params << "-o" << filename;
        params << filename+".asm";
       // qDebug() << params;
        process.start(m_settingsIni->getString("vasmm"), params);
        process.waitForFinished();
        output = process.readAllStandardOutput();
        qDebug() << output;
        output = process.readAllStandardError();


    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

//    qDebug() << "*********" << output;

    text+=output;
}

void SystemM68000::PostProcess(QString &text, QString file, QString currentDir)
{
    QString output=text;



    if (output.toLower().contains("error")) {
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
        if (output.toLower().contains("branch out of range")) {
            Messages::messages.DisplayMessage(Messages::messages.BRANCH_ERROR);
            output += "<br>Please check your <b>onpage/offpage</b> keywords.";

        }
        else
            if (output.toLower().contains("reverse-indexed")) {
                Messages::messages.DisplayMessage(Messages::messages.MEMORY_OVERLAP_ERROR);
                output += "<br>Please reorganize your binary inclusions in ascending order of memory locations.";
            }
            else
                if (output.toLower().contains("mnemonic")) {
                    output += "<br>Please make sure you have used well-defined labels and variables in your inline assembly code.";
                }

                else
                    Messages::messages.DisplayMessage(Messages::messages.DASM_COMPILER_ERROR);

    }
/*    if (!output.toLower().contains("complete.")) {
        m_buildSuccess = false;
        if (output=="") {
            Messages::messages.DisplayMessage(Messages::messages.NO_DASM);

            output = output + "\nCould not find Dasm.exe. Did you set the correct environment variables?";
        }

    }
*/
    text = output;

    if (m_buildSuccess) {

        QString newFile = m_settingsIni->getString("vasmm_target_dir") + "/"+file.split("/").last();
        if (QFile::exists(newFile))
            QFile::remove(newFile);

        qDebug() << file;

        QFile::copy(file, newFile);
    }
}
