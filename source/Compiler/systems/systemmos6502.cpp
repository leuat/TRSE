#include "systemmos6502.h"
#include "source/OrgAsm/orgasm.h"
#include <QElapsedTimer>
#include <QProcess>



void SystemMOS6502::Assemble(QString& text, QString filename, QString currentDir, SymbolTable* symTab)
{


    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;
    //qDebug() << m_settingsIni->getString("assembler");
    if (m_settingsIni->getString("assembler").toLower()=="dasm") {
        QProcess process;
        process.start(m_settingsIni->getString("dasm"), QStringList()<<(filename +".asm") << ("-o"+filename+".prg"));//) << "-v3");
        process.waitForFinished();
        //process;
        //qDebug() <<process.readAllStandardOutput();
        output = process.readAllStandardOutput();

        // codeEnd=FindEndSymbol(output);
    }
    else if (m_settingsIni->getString("assembler").toLower()=="orgasm") {
        Orgasm orgAsm;
        for(QString k: symTab->m_constants.keys()) {
            orgAsm.m_constants[k] = Util::numToHex(symTab->m_constants[k]->m_value->m_fVal);
        }

        orgAsm.Assemble(filename+".asm", filename+".prg");
        output = orgAsm.m_output;

        if (m_projectIni->getdouble("output_debug_symbols")==1.0)
            orgAsm.SaveSymbolsList(filename+".sym");


    }
    // Machine Code Analyzer
    VerifyMachineCodeZP(filename+".prg");

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+".prg").size();

    if (Syntax::s.m_stripPrg)
        Util::ConvertFileWithLoadAddress(filename+".prg", filename+".prg");


    if (m_projectIni->getdouble("exomizer_toggle")==1 && (Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES)) {
        QProcess processCompress;

        QString fn = (filename +".prg");
        QString target="-t64";
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128)
            target="-t128";
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
            target="-t20";
            if (m_projectIni->getString("vic_memory_config")!="none") {
                target="-t52";

            }
        }

        if (!QFile::exists(m_settingsIni->getString("exomizer"))) {
            m_buildSuccess = false;
            text = text + "<br><font color=\"#FF6040\">Incorrect exomizer path. Please setup exomizer in the TRSE settings panel</font><br>";
            return;
        }
//            Messages::messages.DisplayMessage(Messages::messages.NO_EXOMIZER);


        QString startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress);
        if (Syntax::s.m_ignoreSys)
            startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress+1);

        QStringList exoParams = QStringList() << "sfx" << startAddress << target << fn<< "-o" << fn ;

        if (m_settingsIni->getdouble("hide_exomizer_footprint")==1)
            exoParams << "-n";


        /*            if (Syntax::s.m_ignoreSys) {
                QString laddr = "none";
                laddr = "0x8000";
    //                laddr = "0x"+QString::number(Syntax::s.m_startAddress+1+16992    ,16);
                exoParams = QStringList() << "mem" <<"-l"+laddr << fn+"@"+startAddress.replace("$","0x")<< "-o" << fn ;
                qDebug() << exoParams;
                target ="";
            }
    */
        //            QStringList exoParams = QStringList() << "sfx" << "$0810"  << fn<< "-o" << fn ;
        // qDebug() << exoParams;
 //       qDebug() << "Starting exomizer";
        processCompress.start(m_settingsIni->getString("exomizer"), exoParams  );
        processCompress.waitForFinished();
   //     qDebug() << processCompress.readAllStandardError() << processCompress.readAllStandardOutput();
     //   qDebug() << "*** END EXO";
    }


    //        if (Syntax::s.m_stripPrg)
    //         Util::ConvertFileWithLoadAddress(filename+".prg", filename+".prg");


    int size = QFile(filename+".prg").size();
    int crunchTime = timer.elapsed()- time;


    m_buildSuccess = true;
    m_orgOutput = "";
    if (output.toLower().contains("error")) {
        m_orgOutput = output;
        text="<font color=\"#FF6040\">Fatal error during assembly!</font><br>";
        m_buildSuccess = false;
    }

    if (!output.toLower().contains("complete.")) {
        m_buildSuccess = false;
    }

    if (m_buildSuccess) {

        output ="Assembled file size: <b>" + QString::number(size) + "</b> bytes";
        if (m_projectIni->getdouble("exomizer_toggle")==1) {
            output=output+" (<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)size/(float)orgFileSize))+  " % </font> of original size ) <br>";
            output=output+"Original file size: " + QString::number(orgFileSize) + " bytes";
        }
        output = output + "\nAssemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>";
        output = output + "Crunch time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(crunchTime)) + "</font>";

    }


    text+=output;


}

void SystemMOS6502::PostProcess(QString &text, QString filename, QString currentDir)
{
    QString output;
//    TestForCodeOverwrite(codeEnd,text);



    if (m_projectIni->getString("output_type")=="crt") {
        QByteArray output;
        QFile header(":resources/bin/crt_header.bin");
        header.open(QFile::ReadOnly);
        output = header.readAll();

        header.close();
        output[0x99] = (char)m_projectIni->getdouble("background_color");
        output[0x94] = (char)m_projectIni->getdouble("border_color");

        output[0x90] = (char)Util::NumberFromStringHex(m_projectIni->getString("machine_state"));

        QByteArray mainb;
        QFile mainf(filename+".prg");
        mainf.open(QFile::ReadOnly);
        mainb = mainf.readAll();
        mainf.close();
        mainb.remove(0,2);
        output.append(mainb);
        if (mainb.size()>=16384) {
            /*QMessageBox msgBox;
            m_buildSuccess = false;
            msgBox.setText("Error: Compiled file larger than maximum cartridge size (max 16386 bytes vs current "+QString::number(mainb.size()) + " bytes)." );
            msgBox.exec();*/
            text=text + "<br><font color=\"#FF6040\">Error: Compiled file larger than maximum cartridge size (max 16386 bytes vs current "+QString::number(mainb.size()) + " bytes).</font><br>";
            m_buildSuccess = false;

            return;

        }

        for (int i=output.size();i<16464;i++)
            output.append((char)0);

        QFile o(filename+".crt");
        o.open(QFile::WriteOnly);
        o.write(output);
        o.close();

    }

    if (m_projectIni->getString("output_type")=="d64") {
        if (!QFile::exists(m_settingsIni->getString("c1541"))) {
            //Messages::messages.DisplayMessage(Messages::messages.NO_C1541);
            text=text + "<br><font color=\"#FF6040\">link to c1541 not set up in the TRSE settings panel.</font><br>";
            m_buildSuccess = false;
            return;
        }
        CreateDisk(currentDir, filename, "d64_paw_file", true);
        CreateDisk(currentDir,filename+"_side2", "d64_paw_file_disk2",false);
    }


    text+=output;
}

bool SystemMOS6502::VerifyMachineCodeZP(QString fname)
{
   m_mca.Load(fname);
    m_mca.AnalyzeZP();
    return true;
}

void SystemMOS6502::CreateDisk(QString currentDir, QString filename, QString iniData, bool addPrg)
{
    QString f = filename.split("/").last();
    QStringList d64Params = QStringList() << "-format" << f+",id"<< "d64";
    d64Params << filename+".d64";
    d64Params << "-attach" <<filename+".d64";
    if (addPrg)
        d64Params << "-write" <<filename+".prg" << f;

    if (m_projectIni->getString(iniData)!="none") {
        if (!BuildDiskFiles(currentDir, d64Params,iniData))
            return;


        QProcess process1541;
        //     qDebug() <<"Building disk with: " << d64Params;
        process1541.start(m_settingsIni->getString("c1541"), d64Params  );
        process1541.waitForFinished();
    }

}

bool SystemMOS6502::BuildDiskFiles(QString currentDir, QStringList &d64Params, QString iniData)
{

    QString pawFile = m_projectIni->getString(iniData);
    CIniFile paw;
    paw.Load(currentDir + "/"+pawFile);
    QStringList data = paw.getStringList("data");
    QStringList data_tc = paw.getStringList("data_tinycrunch");
    int count = data.count()/3;
    QString outFolder = currentDir+"/"+ paw.getString("output_dir");
    if (!QDir().exists(outFolder))
        QDir().mkdir(outFolder);

    for (int i=0;i<count;i++) {
        QString orgFileName = data[3*i+1];

        bool isCrunched = false;
        if (i<data_tc.count())
            isCrunched = data_tc[i]=="1";

        QString name = data[3*i];

        for (int i=0;i<256;i++) {
            QString r = "#P"+QString::number(i)+";";
            name = name.replace(r,QChar(i));
        }


        int address = Util::NumberFromStringHex( data[3*i+2]);
        QString fn = currentDir+"/"+orgFileName;
        if (!QFile::exists(fn)) {
            //QMessageBox msgBox;
            //msgBox.setText("Error: Could not append disk include file '"+fn+"' because it does not exist");
            //msgBox.exec();
            //text=text + "<br><font color=\"#FF6040\">Error: Could not append disk include file '"+fn+"' because it does not exist</font><br>";
            m_buildSuccess = false;

            return false;
        }
        if (!isCrunched) {
            QString of = outFolder+"/"+orgFileName.split("/").last();
            Util::ConvertFileWithLoadAddress(fn,of,address);
            //        Util::CopyFile(fn,of);

            d64Params << "-write" <<of << name;
        }
        else {
            //            QString ending = orgFileName.split(".").last();
            QString iff = name;
            QString of = outFolder+"/"+iff+"_c.bin";
            //            qDebug() << of;
            d64Params << "-write" <<of << name;

        }
    }

    return true;

}

