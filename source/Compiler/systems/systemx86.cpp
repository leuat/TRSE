#include "systemx86.h"

void SystemX86::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab)
{
    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    if (!QFile::exists(m_settingsIni->getString("nasm"))) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the NASM assembler in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }

    //qDebug() << m_settingsIni->getString("assembler");
        QProcess process;
        QStringList params;
        bool qemu = false;
        if (m_projectIni->contains("qemu") && m_projectIni->getString("qemu").startsWith("qemu")) {
            params << "-f"<< "bin";
            params << filename+".asm";
            params << "-o" << filename + ".bin";
            qemu = true;
        }
        else
        {
            params << "-o" << filename + ".exe";
            params << filename+".asm";
        }
        process.start(m_settingsIni->getString("nasm"), params);
        process.waitForFinished();
        output = process.readAllStandardOutput();
        output+= process.readAllStandardError();


        if (qemu) {
            QByteArray boot = Util::loadBinaryFile(":resources/bin/bootsect.bin");
//            qDebug() <<boot.count();
            QByteArray data = Util::loadBinaryFile(filename+".bin");
            boot.append(data);
            Util::SaveByteArray(boot,filename+".bin");

        }





    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

//    qDebug() << "*********" << output;

    text+=output;
}

void SystemX86::PostProcess(QString &text, QString file, QString currentDir)
{
    QString output=text;



    if (output.toLower().contains("error")) {
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
        if (output.toLower().contains("branch out of range")) {
  //          Messages::messages.DisplayMessage(Messages::messages.BRANCH_ERROR);
            output += "<br>Please check your <b>onpage/offpage</b> keywords.";

        }
        else
            if (output.toLower().contains("reverse-indexed")) {
//                Messages::messages.DisplayMessage(Messages::messages.MEMORY_OVERLAP_ERROR);
                output += "<br>Please reorganize your binary inclusions in ascending order of memory locations.";
            }
            else
                if (output.toLower().contains("mnemonic")) {
                    output += "<br>Please make sure you have used well-defined labels and variables in your inline assembly code.";
                }

//                else
  //                  Messages::messages.DisplayMessage(Messages::messages.DASM_COMPILER_ERROR);

    }
/*    if (!output.toLower().contains("complete.")) {
        m_buildSuccess = false;
        if (output=="") {
            Messages::messages.DisplayMessage(Messages::messages.NO_DASM);

            output = output + "\nCould not find Dasm.exe. Did you set the correct environment variables?";
        }

    }
*/

    if (m_buildSuccess) {

            int orgFileSize = QFile(file).size();

            output+="<br>Assembled file size: <b>" + QString::number(orgFileSize) + "</b> bytes";
/*            if (m_settingsIni->getdouble("perform_crunch")==1) {
                output=output+" (<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)size/(float)orgFileSize))+  " % </font> of original size ) <br>";
                output=output+"Original file size: " + QString::number(orgFileSize) + " bytes";
            }
            output = output + "\nAssemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>";
            output = output + "Crunch time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(crunchTime)) + "</font>";

  */


/*        QString newFile = m_settingsIni->getString("vasmm_target_dir") + "/"+file.split("/").last();
        if (QFile::exists(newFile))
            QFile::remove(newFile);

//        qDebug() << file;

        QFile::copy(file, newFile);*/
    }
    text = output;

}

bool SystemX86::is486()
{
    if (m_cpu=="8088" || m_cpu=="8086" || m_cpu=="286" || m_cpu=="386")
        return false;

    return true;
}

bool SystemX86::is386()
{
    if (m_cpu=="8088" || m_cpu=="8086" || m_cpu=="286")
        return false;

    return true;

}

bool SystemX86::is286()
{
    if (m_cpu=="8088" || m_cpu=="8086")
        return false;

    return true;

}

bool SystemX86::is8088()
{
    return m_cpu=="8088";
}
