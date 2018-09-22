#include "mainwindow.h"
#include <QApplication>
#include <QStyleFactory>



void SetMongisPalette(QApplication& a) {
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(128,53,255));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Base, QColor(25,25,25));
    darkPalette.setColor(QPalette::AlternateBase, QColor(53,53,53));
    darkPalette.setColor(QPalette::ToolTipBase, Qt::white);
    darkPalette.setColor(QPalette::ToolTipText, Qt::white);
    darkPalette.setColor(QPalette::Text, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(53,53,53));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(42, 130, 218));

    darkPalette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    darkPalette.setColor(QPalette::HighlightedText, Qt::black);

    a.setStyleSheet("QToolTip { color: #ffffff; background-color: #2a82da; border: 1px solid white; }");

    a.setPalette(darkPalette);


}

void ConvertPerlin(QString input, QString out, float div) {
    QImage img;
    img.load(input);

    QByteArray a;
    int xw = img.width()/div;
    int xh = img.height()/div;
    a.resize(xw*xh);

    qDebug() << "Converting file: " << xw;

    for (int x=0;x<xw;x++)
        for (int y=0;y<xh;y++) {

            a[x + y*xw]=QColor(img.pixel(x*div, y*div)).red();
            if (rand()%100>98)
                qDebug() << a[x + y*xw];
        }

    QFile f(out);
    f.open(QFile::WriteOnly);
    f.write(a);
    f.close();
}



int main(int argc, char *argv[])
{

/*   #pragma omp for

    for (int i=0;i<19;i++)
        i=0;*/
    QApplication a(argc, argv);
    a.setStyle(QStyleFactory::create("Fusion"));
    //CIniFile ini;

//    SetMongisPalette(a);



//    ConvertPerlin("/home/leuat/Desktop/Perlin.png", "/home/leuat/code/TRSE/Publish/Pluttifer/data/perlin16.dat", 8);



    MainWindow w;
    w.show();

    return a.exec();
}
