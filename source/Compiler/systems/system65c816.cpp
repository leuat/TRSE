#include "system65c816.h"

System65C816::System65C816(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : AbstractSystem(settings, proj) {
    m_allowedGlobalTypeFlags << "compressed"<<"pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert"<<"bank"<<"lpointer";
    m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"invert" <<"global" <<"stack"<<"lpointer";
    m_supportsExomizer = true;
    m_registers << "_a"<<"_x" <<"_y" <<"_ax" <<"_ay" <<"_xy";
    m_canRunAsmFiles = true;
    m_allowClasses = true; // EXPERIMENTAL
    m_processor = WDC65C816;

    m_allowedBaseTypes<<"LONG";
    m_renameVariables << "a" <<"x"<<"y"<<"pc"<<"sp";

}

void System65C816::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{




    if (m_projectIni->getdouble("exomizer_toggle")==1) {
        QProcess processCompress;

        QString fn = (filename +".prg");
        QString target="-t65";

        if (!QFile::exists(m_settingsIni->getString("exomizer"))) {
            m_buildSuccess = false;
            text = text + "<br><font color=\"#FF6040\">Incorrect exomizer path. Please setup exomizer in the TRSE settings panel</font><br>";
            return;
        }
//            Messages::messages.DisplayMessage(Messages::messages.NO_EXOMIZER);


        QString startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress);
        if (Syntax::s.m_ignoreSys)
//            startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress+1);
          startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress);
//        startAddress="$2001";
  //      qDebug() << startAddress;
        QStringList exoParams = QStringList()<<  "sfx" << startAddress << target << fn<< "-o" << fn;

        if (m_settingsIni->getdouble("hide_exomizer_footprint")==1)
            exoParams << "-n";

        emit EmitTick("Exomizing ...");

        processCompress.start(m_settingsIni->getString("exomizer"), exoParams  );
        processCompress.waitForFinished();
//        qDebug() << processCompress.readAllStandardError();
  //      qDebug() <<processCompress.readAllStandardOutput();
    }


}


void System65C816::PostProcess(QString &text, QString filename, QString currentDir)
{
    QString output;
    int disk = 1;
    int track = 18;
    if (m_projectIni->getdouble("use_track_19")==1.0) track=19;
    if (m_projectIni->getString("output_type")=="d64") {
        while (m_projectIni->contains("disk"+QString::number(disk)+"_paw")) {
            QString d = "disk"+QString::number(disk);;
            if (!CreateDiskInternal(currentDir, d,filename, disk==1,output,track)) {
                text+=output;
                return;
            }
            disk+=1;
        }
        if (disk==1) {
            text +="<br>You need to set up at least one disk in the project settings.";
            m_buildSuccess = false;
            return;
        }
    }




    text+=output;

}

void System65C816::DefaultValues()
{
    m_ignoreSys = false;
    m_startAddress=0x800;
}
