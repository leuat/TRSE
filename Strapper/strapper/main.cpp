#include <QProcess>
#include <iostream>
#include <QStringlist>
#include "updater.h"
using namespace std;

float currentVersion = 0.009;

void CheckVersion() {
    Updater u;
    u.Initialize();
    if (!u.CheckVersionOK(currentVersion)) {
        qDebug() << "New version of Turbo Rascal Syntax Error (" + QString::number(u.m_newVersion) +") released, downloading...";
        u.FetchNewVersion();

        //QProcess::startDetached("bin/7z.exe", QStringList() << "x"<<"trse_win.zip" <<"-aoa" << "-O..\\", "." );
/*        if (!QFile::exists("newVersion.cmd")) {
            QProcess p;
            cout << "Error : New Bootstrapper version. Trying to extract newVersion.cmd. If this doesn't work, just unpack the zip file manually.. hopefully works in the next version" << endl;
            p.execute("bin/7z.exe", QStringList() << "x"<<"trse_win.zip" <<"-aoa" << "-O..\\"<< "newVersion.cmd");
        }*/



    }
    else qDebug() << "No new version. Continuing.";
}


int main(int argc, char *argv[])
{
    cout << "This is TRSE updater currently version " << QString::number(currentVersion).toStdString() << endl;
    CheckVersion();
//    QProcess::startDetached("bin/trse.exe",QStringList(),"bin/");
    QProcess::startDetached("start.cmd",QStringList(),".");
}
