#include "systemmos6502.h"
#include "source/OrgAsm/orgasm.h"
#include <QElapsedTimer>
#include <QProcess>
#include "source/LeLib/limage/c64fullscreenchar.h"
#include "source/LeLib/limage/limageio.h"
#include "source/LeLib/util/cc1541.h"
#include "source/LeLib/util/dirartd64.h"


void SystemMOS6502::Assemble(QString& text, QString filename, QString currentDir, QSharedPointer<SymbolTable>  symTab)
{


    QString output;
    int time = timer.elapsed();
    int codeEnd = 0;

    m_buildSuccess = true;
    AssembleTripe(text,filename,currentDir,symTab);
    if (!m_buildSuccess)
        return;

    //qDebug() << m_settingsIni->getString("assembler");
    if (m_settingsIni->getString("assembler").toLower()=="dasm") {
        emit EmitTick("<br>Assembling with DASM ...");
        QProcess process;
        process.start(m_settingsIni->getString("dasm"), QStringList()<<(filename +".asm") << ("-o"+filename+".prg"));//) << "-v3");
        process.waitForFinished();
        //process;
        //qDebug() <<process.readAllStandardOutput();
        output = process.readAllStandardOutput();

        // codeEnd=FindEndSymbol(output);
    }
    else if (m_settingsIni->getString("assembler").toLower()=="orgasm") {

        AssembleOrgasm(output,text,filename,currentDir,symTab);


    }
    // Machine Code Analyzer
    VerifyMachineCodeZP(filename+".prg");

    int assembleTime = timer.elapsed()- time;
    time = timer.elapsed();

    //        qDebug() << "Code end: " << Util::numToHex(codeEnd) << codeEnd;
    int orgFileSize = QFile(filename+".prg").size();

    //    qDebug() << "MOS " << Syntax::s.m_stripPrg << Syntax::s.m_ignoreSys;

    if (Syntax::s.m_currentSystem->m_stripPrg)
        Util::ConvertFileWithLoadAddress(filename+".prg", filename+".prg");


    //    qDebug() << "Exomizer toggle" <<m_projectIni->getdouble("exomizer_toggle");
    if (m_projectIni->getdouble("exomizer_toggle")==1 && (Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES)) {
        QProcess processCompress;

        QString fn = (filename +".prg");
        QString target="-t64";
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C128)
            target="-t128";
        if (Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {
            target="-t20";
            if (m_projectIni->getString("temp_vic_memory_config")!="none") {
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
            //            startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress+1);
            startAddress = Util::numToHex(Syntax::s.m_currentSystem->m_startAddress);

        QStringList exoParams = QStringList() << "sfx" << startAddress << target << fn<< "-o" << fn ;

        if (m_settingsIni->getdouble("hide_exomizer_footprint")==1)
            exoParams << "-n";

        emit EmitTick("Exomizing ...");

        processCompress.start(m_settingsIni->getString("exomizer"), exoParams  );
        processCompress.waitForFinished();
    }




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
        output = output.remove("Complete.");
        output = output + "Assemble time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(assembleTime)) + "</font>. ";
        if (m_projectIni->getdouble("exomizer_toggle")==1) {
            output=output+" (<font color=\"#70FF40\"> " + QString::number((int)(100.0*(float)size/(float)orgFileSize))+  " % </font> of original size ) <br>";
            output=output+"Original file size: " + QString::number(orgFileSize) + " bytes.";
            output = output + "Crunch time: <font color=\"#70FF40\">"+ (Util::MilisecondToString(crunchTime)) + "</font>";
        }

        output+="Assembled file size: <b>" + QString::number(size) + "</b> bytes. ";

    }


    text+=output;


}

void SystemMOS6502::PostProcess(QString &text, QString filename, QString currentDir)
{
    QString output;
    //    TestForCodeOverwrite(codeEnd,text);

    /*
    *
    *
    * VIC-20
    * $A000-A001 contains start address of the program at reset
$A002-A003 contains address where to jump pressing restore
$A004 -> contains
a0CBM
10:56
or
65,48,195,194,205
as decimal
10:56
@Leuat it's difference in the sense that you have different blocks
10:56
and ONLY BLK5 can autostart cart
10:56
BLK5 = $A000-$BFFF
    *
    *
    * */

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
/*        if (!QFile::exists(m_settingsIni->getString("c1541"))) {
            //Messages::messages.DisplayMessage(Messages::messages.NO_C1541);
            text=text + "<br><font color=\"#FF6040\">link to c1541 not set up in the TRSE settings panel.</font><br>";
            m_buildSuccess = false;
            return;
        }*/
        if (m_projectIni->getdouble("use_vice_c1541")!=1) {
            CreateDiskInternal(currentDir, filename, "d64_paw_file", true,output,m_projectIni->getString("d64name"));
            CreateDiskInternal(currentDir,filename+"_side2", "d64_paw_file_disk2",false,output,m_projectIni->getString("d64name"));
        }
        else {
            CreateDiskC1541(currentDir, filename, "d64_paw_file", true,output);
            CreateDiskC1541(currentDir,filename+"_side2", "d64_paw_file_disk2",false,output);

        }

    }


    text+=output;
}

bool SystemMOS6502::VerifyMachineCodeZP(QString fname)
{
    m_mca.Load(fname);
    m_mca.AnalyzeZP();
    return true;
}

// OBSOLETE
void SystemMOS6502::CreateDiskC1541(QString currentDir, QString filename, QString iniData, bool addPrg, QString& text)
{
    QString f = filename.split("/").last();
    QStringList d64Params = QStringList();
    d64Params << "-format" << f+",id"<< "d64";
    d64Params << filename+".d64";
    d64Params << "-attach" <<filename+".d64";
    if (addPrg)
        d64Params << "-write" <<filename+".prg" << f;

    if (m_projectIni->getString(iniData)!="none") {
        if (!BuildDiskFiles(currentDir, d64Params,iniData)) {
            text+="<br><font color=\"#FF8080\">Error</font>! Could not build C64 disk.. please make sure that all the files specified in "+filename+" exist!<br>";
            //            qDebug() << "SystemMos6502 : Could not build disk!";
            return;
        }
        QProcess process1541;
        //     qDebug() <<"Building disk with: " << d64Params;
        process1541.start(m_settingsIni->getString("c1541"), d64Params  );
        process1541.waitForFinished();
        qDebug() << d64Params;
        qDebug() << "c1541 output";
        qDebug() << process1541.readAllStandardError();
        qDebug() << process1541.readAllStandardOutput();

    }

}

void SystemMOS6502::CreateDiskInternal(QString currentDir, QString filename, QString iniData, bool addPrg, QString& text, QString diskName)
{
    QString f = filename.split("/").last();
    QStringList d64Params = QStringList();


    QString type = m_projectIni->getString("cc1541_disk_type");

    if (QFile::exists(filename+"."+type))
        QFile::remove(filename+"."+type);



    QStringList shadowDir = QStringList()<<"-d" <<"19";
    // Create a disk

    QStringList cd64;
    cd64<<"cc1541";
    d64Params <<"cc1541";
    cd64 << "-n" << diskName <<shadowDir;
    cd64 << filename+"."+type;
    // call
//    cc1541(cd64.size(), Util::StringListToChar(cd64));


    // Start building files...
    d64Params  <<"-d"<<"19" <<"-n" << diskName << shadowDir;

    if (addPrg)
        d64Params << "-f"<<f << "-w"<<filename+".prg";


    if (m_projectIni->getString(iniData)!="none") {
        if (!BuildDiskFilesCC1541(currentDir, d64Params,iniData, text))
        {
            text+="<br><font color=\"#FF8080\">Error</font>! Could not build C64 disk.. please make sure that all the files specified in "+filename+" exist!<br>";
            return;
        }
        d64Params<<filename+"."+type;
        qDebug() << d64Params;
        cc1541(d64Params.size(), Util::StringListToChar(d64Params));
        std::cout << stdout;
        std::cout << stderr;
    }

    if (QFile::exists(filename+"."+type)) {
//        qDebug() << "Applying dir art to " +filename;

        QString type = m_projectIni->getString("cc1541_disk_type");
        ApplyDirArt(currentDir,m_projectIni->getString("dirart_flf_file"),filename+"."+type, text);
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

    QString dirart = m_projectIni->getString("dirart_flf_file");
    QVector<QString> filenames;
    QVector<QByteArray> filenames_raw;


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
        QString oname = name;
        // argh
        //        if (i<filenames.count())
        //          oname = filenames[i];//+",s";

        if (!isCrunched) {
            QString of = outFolder+"/"+orgFileName.split("/").last();
            Util::ConvertFileWithLoadAddress(fn,of,address);
            //        Util::CopyFile(fn,of);

            d64Params << "-write" <<of << oname;
        }
        else {
            //            QString ending = orgFileName.split(".").last();
            QString iff = name;
            QString of = outFolder+"/"+iff+"_c.bin";
            //            qDebug() << of;
            d64Params << "-write" <<of << oname;
            //            if (!orgFileName.toLower().endsWith(".prg"))
            //             Util::ConvertFileWithLoadAddress(of,of,address);

        }
    }

    return true;

}

bool SystemMOS6502::BuildDiskFilesCC1541(QString currentDir, QStringList &d64Params, QString iniData, QString& text)
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


        if (!Syntax::s.StringIsAlnum(name) || name.contains("_")) {
            text=text + "<br><font color=\"#FF6040\">Error: Filename '"+name+"' is incorrect since it must be alphanumerical and not contain other characters (a-z-0-9)</font><br>";
            m_buildSuccess = false;
            return false;

        }


        int address = Util::NumberFromStringHex( data[3*i+2]);
        QString fn = currentDir+"/"+orgFileName;
        if (!QFile::exists(fn)) {
            text=text + "<br><font color=\"#FF6040\">Error: Could not append disk file '"+fn+"' because it does not exist</font><br>";
            m_buildSuccess = false;

            return false;
        }
        QString oname = name;

        if (!isCrunched) {
            QString of = outFolder+"/"+orgFileName.split("/").last();
            Util::ConvertFileWithLoadAddress(fn,of,address);
            d64Params << "-f" <<oname << "-w" <<of;
        }
        else {
            //            QString ending = orgFileName.split(".").last();
            QString iff = name;
            QString of = outFolder+"/"+iff+"_c.bin";
            if (!QFile::exists(of)) {
                text=text + "<br><font color=\"#FF6040\">Error: Could not append compressed disk file '"+of+"' because it does not exist. Did you build your disk before compiling?</font><br>";
                m_buildSuccess = false;
                return false;
            }
            d64Params << "-f" << oname << "-w" <<of;
        }
    }

    return true;

}

void SystemMOS6502::ApplyDirArt(QString currentDir, QString dirart, QString diskf, QString &text)
{
    if (dirart=="none")
        return;

    if (!QFile::exists(currentDir+dirart))
        return;

    LImage* img = LImageIO::Load(currentDir + dirart);
    QString dirartfn = currentDir + "dirart.bin";
    QByteArray art = img->getDirArt();

    while (art.size()>0 && ((uchar)art[art.size()-1]==(uchar)0x20))
        art.remove(art.size()-1,1);
    Util::SaveByteArray(art,dirartfn);


    DirArtD64 da;
    QStringList p;
#ifdef _WIN32
#else
#endif
    p<<"";


    p  << "-b" << dirartfn << diskf << diskf;
    try {

        da.Write(p.count(), Util::StringListToChar(p));

    } catch (QString s) {
        text+="<br><font color=\"#FF6040\">Error: Could not apply dir art: "+s+"</font><br>";
        m_buildSuccess = false;
    }


}


void SystemMOS6502::DefaultValues()
{
    m_startAddress = 0x2001;
    m_programStartAddress = 0x2020;
    m_ignoreSys = false;
    m_stripPrg = false;

}

QStringList SystemMOS6502::CompressData(QStringList& inData, QString& string) {
    QByteArray in;
    for (auto& s: inData) {
        in.append(Util::NumberFromStringHex(s));
    }
    for (QChar s: string)
        in.append(s.toLatin1());

    QString filename = "_ctemp.bin";
    Util::SaveByteArray(in,filename);
    QString fn2 =  CompressLZ4(filename);
    QByteArray comp = Util::loadBinaryFile(fn2);
    QFile::remove(fn2);
    QFile::remove(filename);

    QStringList lst;
    for (int i=0;i<comp.length();i++)
        lst.append(Util::numToHex((unsigned char)comp[i]));

    return lst;

}

void SystemMOS6502::applyEmulatorParametersVICE(QStringList &params, QString debugFile, QString filename) {
    if (QFile::exists(debugFile))
        params<<"-moncommands"<<debugFile;
    if (m_settingsIni->getdouble("auto_inject")==1.0) {
        params << "-autostartprgmode" << "1";
    }
    auto type=m_projectIni->getString("output_type");
    if (type!="d64")
        params << filename+"."+type;
    else
        params << filename+"."+m_projectIni->getString("cc1541_disk_type");

}

void SystemMOS6502::PrepareInitialAssembler(Assembler *as) {
    // new method
    //Asm(".byte $00 ; fill $xxx0");
    if (!(!Syntax::s.m_ignoreSys && (Syntax::s.m_currentSystem->m_programStartAddress!=Syntax::s.m_currentSystem->m_startAddress)))
        return;



    as->Asm( ".byte $" + QString::number( (Syntax::s.m_currentSystem->m_startAddress + 10) & 0x0ff, 16  ) + " ; lo byte of next line" );
    as->Asm( ".byte $" + QString::number( ( (Syntax::s.m_currentSystem->m_startAddress + 10) & 0x0ff00 ) >> 8, 16 ) + " ; hi byte of next line" );
    as->Asm(".byte $0a, $00 ; line 10 (lo, hi)");
    as->Asm(".byte $9e, $20 ; SYS token and a space");
    // write PETSCII / ASCII representation of address to call
    as->Asm(Util::IntToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
    as->Asm(".byte $00, $00, $00 ; end of program");

    /* // old method
        Asm(".byte    $0, $0E, $08, $0A, $00, $9E, $20");
        Asm(intToHexString(Syntax::s.m_currentSystem->m_programStartAddress));
        Asm(".byte     $00");   // 6, 4, )
        */
    as->Nl();

    as->EndMemoryBlock();
    //        Comment("End of SYS memory block, starting new");
    as->StartMemoryBlock(Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress));
    as->m_insertEndBlock = "EndBlock"+Util::numToHex(Syntax::s.m_currentSystem->m_programStartAddress).remove("$");
    //      Comment("Start of MAIN BLOCK");
    //        qDebug() << "INSERT "+m_insertEndBlock;

}

