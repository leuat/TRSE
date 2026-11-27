/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/
#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QSettings>
#ifdef TRSE_QT56
#include <QTextCodec>
#endif
#include "source/misc/cli.h"

void fixCurrentDir(QString execFile) {
    QFileInfo exec(execFile);
    QDir::setCurrent(exec.absoluteDir().absolutePath());
}


int main(int argc, char *argv[])
{
#ifdef TRSE_QT56
    // Windows XP build only: Qt 5.6 text streams default to the system locale codec (cp1252),
    // Qt 6 to UTF-8. Use UTF-8 so source files and generated output match the Qt 6 builds.
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
#endif

#ifdef _WIN32
/*    // Make sure that stdout attaches itself to the console window on win32 for cli stuff
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
    }
*/
#endif
    // run TRSE as CLI
    if (argc>=2) {
        if (QString(argv[1])=="-cli") {
            ClascExec ras(argc, argv);
            return ras.Perform();
        }
    }
    QApplication a(argc, argv);
    a.setOrganizationDomain("lemonspawn.com");
    a.setApplicationName("TRSE");
    QString oldCurDir = QDir::currentPath();
    fixCurrentDir(QString(argv[0]));
    a.setStyle(QStyleFactory::create("Fusion"));
    MainWindow w;
    for (int i=0; i<argc;i++)
        w.m_commandParams+=QString(argv[i]);
    w.show();
    w.AfterStart(oldCurDir);
    w.RestoreSettings();
    return a.exec();
}
