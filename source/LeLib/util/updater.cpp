#include "updater.h"
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
Updater::Updater()
{

}

void Updater::Initialize()
{
    int a = 1;
    QCoreApplication app(a, nullptr);

    if (QFile::exists(m_versionFile)) {
        qDebug() << "Removing old version file..";
        QFile::remove(m_versionFile);

    }

    QUrl versionUrl(m_versionURL);
    DownloadManager dm;
    dm.append(versionUrl);
    dm.saveFileName(versionUrl);
    //dm.st

    QObject::connect(&dm, SIGNAL(finished()), &app, SLOT(quit()));
    app.exec();
}

void Updater::FetchNewVersion()
{
    int a = 1;
    QCoreApplication app(a, nullptr);

    if (QFile::exists(m_appFile)) {
        qDebug() << "Removing old downloaded version..";
        QFile::remove(m_appFile);

    }

    QUrl appUrl(m_appURL);
    DownloadManager dm;
    dm.append(appUrl);
    dm.saveFileName(appUrl);
    //dm.st

    QObject::connect(&dm, SIGNAL(finished()), &app, SLOT(quit()));
    app.exec();

}

bool Updater::CheckVersionOK(float currentVersion)
{
    QFile file(m_versionFile);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Could not open file.. assuming no internet connection";
        return true;
    }
    QTextStream in(&file);
    QString line = in.readLine();
    m_newVersion = line.toFloat();
    file.close();

    if (m_newVersion == currentVersion)
        return true;

    return false;

}
void Updater::loadVersion()
{
 //buttonImage.loadFromData(m_pImgCtrl->downloadedData());

}
