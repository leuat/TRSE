#ifndef UPDATER_H
#define UPDATER_H

#include "downloadmanager.h"

class Updater
{

public:

    QString m_versionURL = "https://www.irio.co.uk/trse/version.txt";
    QString m_appURL = "https://www.irio.co.uk/trse/trse_win.zip";
    QString m_appFile = "trse_win.zip";
    QString m_versionFile = "version.txt";
    float m_newVersion;
    Updater();
    void Initialize();
    void FetchNewVersion();
    bool CheckVersionOK(float currentVersion);

private slots:
 void loadVersion();


};

#endif // UPDATER_H
