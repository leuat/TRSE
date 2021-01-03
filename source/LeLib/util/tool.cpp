#include "tool.h"
#include <QProcess>


Tool::Tool()
{

}

bool Tool::AKGCompiler(QString filename, int Address, SymbolTable *symTab)
{
    QString path = QFileInfo(filename).path() + QDir::separator();

    if ((!QFile::exists(filename+".asm")) && (!QFile::exists(filename+".inc")))
        return false;

    if ((QFile::exists(filename+".asm"))) {
        Util::CopyFile(filename+".asm",filename+".inc");
        if ((QFile::exists(filename+"_playerconfig.asm")))
            Util::CopyFile(filename+"_playerconfig.asm",filename+"_playerconfig.inc");

    }

//    qDebug() << path << QFile::exists(":resources/bin/rasm.exe");
    QString player = path+"playerakg.asm";
    QString snd = path+"playerakg_soundeffects.asm";
    QString asmFile = path+"temp.asm";

#ifdef __linux__
    QString rasm = path+"rasm";
    Util::CopyFile(":resources/bin/rasm",rasm);
    QFile::setPermissions(rasm,QFile::ExeUser);
#endif
#ifdef _WIN32
    QString rasm = path+"rasm.exe";
    Util::CopyFile(":resources/bin/rasm.exe",rasm);
#endif
#ifdef __APPLE__
    QString rasm = path+"rasm";
    Util::CopyFile(":resources/bin/rasm_osx",rasm);
    QFile::setPermissions(rasm,QFile::ExeUser);
#endif


    Util::CopyFile(":resources/code/amstrad/playerakg.asm",player);
    Util::CopyFile(":resources/code/amstrad/playerakg_soundeffects.asm",snd);

    QFile file(asmFile);
    file.open(QIODevice::WriteOnly);
    QTextStream qout(&file);
    qout<< "org " + Util::numToHex(Address)<< "\n";
    qout<<"include \""+filename+".inc\" \n";
    qout<<"include \""+filename + "_playerconfig.inc\" \n";


    qout<<"PLY_AKG_HARDWARE_CPC = 1\n";
    qout<<"include \""+player+"\" \n";

    file.close();

    QProcess process;
    process.start(rasm,QStringList() << asmFile << "-o" <<filename << "-s"<<"-sl"<<"-sq");
    process.waitForFinished();
//    qDebug() << process.readAllStandardError() << process.readAllStandardOutput();

    // Ok. We have the binary file - now we need to read the symbol file.



    QStringList symFile = Util::loadTextFile(filename+".sym").split("\n");
    for (QString s: symFile) {
        QStringList d = s.trimmed().simplified().split(" ");
        if (d.count()>1)
            d[1] = d[1].remove("#");
        bool ok;
        if (d[0]=="PLY_AKG_INIT") {
            symTab->m_constants["INIT_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
//            qDebug() << "INIT MUSIC IS " << Util::numToHex(symTab->m_constants["INIT_MUSIC"]->m_value->m_fVal);
        }
        if (d[0]=="PLY_AKG_PLAY") {
            symTab->m_constants["PLAY_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
       }
        if (d[0]=="PLY_AKG_STOP") {
            symTab->m_constants["STOP_MUSIC"]->m_value->m_fVal = d[1].toInt(&ok,16);
        }

    }
    //qDebug() << Util::numToHex(symTab->m_constants["PLAY_MUSIC"]->m_value->m_fVal);

  //  qDebug() << process.readAllStandardError() << process.readAllStandardOutput();
    // Compile time


    QFile::remove(rasm);
    QFile::remove(filename+".sym");
    QFile::remove(player);
    QFile::remove(snd);
    QFile::remove(asmFile);
//    qDebug() << rasm << QFile::exists(rasm);


    if (symTab->m_constants["INIT_MUSIC"]->m_value->m_fVal == 0 ||symTab->m_constants["PLAY_MUSIC"]->m_value->m_fVal==0) {
        ErrorHandler::e.Error("Could not extract music locations. Did RASM actually run?");
    }

    return true;
}

void Tool::PathTool(QString svgFile, QString outBinary, int dataCount, float degreeFrame)
{
    QString svg = Util::loadTextFile(svgFile).trimmed().simplified();
    QStringList coords = svg.split(" ");
    qDebug().noquote() << coords;
    std::vector<QVector3D> lst;
    for (int i=0;i<coords.count();i+=3) {
        QStringList l = coords[i].split(",");
        QVector3D p;
        p.setX(l[0].toFloat());
        p.setY(l[1].toFloat());

        lst.push_back(p);
    }

    Spline2D sp(4);

    sp.set_ctrl_points(lst);

    QByteArray X,Y,T;
    for (int i=0;i<dataCount;i++) {
        float c = 1.0-i/(float)dataCount;
        QVector3D p = sp.eval_f(c);
//        qDebug() << p;
        X.append((char)(((int)(p.x())>>8)&0xFF));
        X.append((char)((int)p.x()&0xFF));
        Y.append((char)(((int)(p.y())>>8)&0xFF));
        Y.append((char)((int)p.y()&0xFF));

        float delta = 0.01;
        QVector3D dir = sp.eval_f(c - delta) - sp.eval_f(c+delta);
        float deg = atan2(dir.y(), dir.x())/3.14159264;
        deg = deg*degreeFrame + (degreeFrame/2.0);


        T.append((char)(((int)(deg)>>8)&0xFF));
        T.append((char)((int)deg&0xFF));
    }
    Util::SaveByteArray(X,outBinary+"x.bin");
    Util::SaveByteArray(Y,outBinary+"y.bin");
    Util::SaveByteArray(T,outBinary+"t.bin");

}
