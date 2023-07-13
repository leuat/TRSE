#include "systembk0010.h"



SystemBK0010::SystemBK0010(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : SystemPDP11(settings, proj) {
    m_system = BK0010;
    m_hasVariableColorPalette = false;
    m_requireEmulatorWorkingDirectory = true;
}

void SystemBK0010::PostProcess(QString &text, QString file, QString currentDir)
{
    QString output=text;
    file+=".bin";

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


        QString newFile = m_settingsIni->getString("vasmm_target_dir") + "/"+file.split("/").last();
        if (QFile::exists(newFile))
            QFile::remove(newFile);

        //        qDebug() << file;

        QFile::copy(file, newFile);
    }
    text = output;
    m_currentDir = currentDir;

}

void SystemBK0010::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {

//    if (!QFile::exists(m_currentDir+"/bk2010.jar"))
  //      Util::CopyFile(":resources/bin/bk2010-0.7.jar",m_currentDir+"/bk2010.jar");

    //java -jar bk2010-0.7.jar -bk0010 -multicolor on -bin "files/test.bin"
    params << "-jar"<< m_settingsIni->getString("bk0010_emulator") << "-bk0010" << "-multicolor on" << "-bin" <<filename+".bin" ;

//    QFile::remove(m_currentDir+"/bk2010.jar");
}
