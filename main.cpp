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
#include "source/LeLib/limage/multicolorimage.h"
void CreateVICCharset() {
    QByteArray ba;
    for (int i=0;i<256;i++) {
        PixelChar pc;
        for (int j=0;j<8;j++) {
            //0000
            //0000
            if (((i>>j)&1)==1) {
                int x = j%4;
                int y = (int)(j/4);
                for (int dy=0;dy<4;dy++)
                    for (int dx=0;dx<2;dx++)
                        pc.set(x*2+dx,y*4+dy,1,0b1);
            }
        }
        for (int i=0;i<8;i++)
            ba.append(PixelChar::reverse(pc.p[i]));
    }
    Util::SaveByteArray(ba,"/Users/leuat/code/TRSE/Publish/tutorials/VIC20/tutorials/resources/charsets/gen.bin");

}

void CreateMergedTorus() {

//    QString p = "SPECTRUM/tutorials";
    QString p = "TIM/tutorials";

    auto b1 = Util::loadBinaryFile("/Users/leuat/code/TRSE/Publish/tutorials/"+p+"/data/inside_torus.bin");
    auto b2 = Util::loadBinaryFile("/Users/leuat/code/TRSE/Publish/tutorials/"+p+"/data/inside_torus2.bin");
    QByteArray d;
    for (int i=0;i<b1.size();i++)
        d.append(((b2[i]/16)&15) | (((b1[i]/8)&15)<<4));

    Util::SaveByteArray(d,"/Users/leuat/code/TRSE/Publish/tutorials/"+p+"/data/combined.bin");


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
//    CreateVICCharset();
//    CreateMergedTorus();
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
