#ifndef UPDATER_H
#define UPDATER_H

#include "source/util/downloadmanager.h"

class Updater
{

public:

    QString m_versionURL = "https://www.irio.co.uk/med/nutil/version.txt";
    QString m_appURL = "https://www.irio.co.uk/med/nutil/nutil_win.zip";
    QString m_appFile = "nutil_win.zip";
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
