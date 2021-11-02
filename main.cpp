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
#include "trc.h"

void fixCurrentDir(QString execFile) {
    QFileInfo exec(execFile);
    QDir::setCurrent(exec.absoluteDir().absolutePath());
}

int main(int argc, char *argv[])
{
    // Execute CLI
    if (argc>=2) {
        if (QString(argv[1])=="-cli") {
            ClascExec ras(argc, argv);
            return ras.Perform();
        }
    }

    // Start main application
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
