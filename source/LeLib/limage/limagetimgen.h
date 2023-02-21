#ifndef LIMAGETIMGEN_H
#define LIMAGETIMGEN_H

#include "limagetim.h"

class LImageTIMGen : public LImageTIM
{
public:
    LImageTIMGen(LColorList::Type t);


    void LoadBin(QFile &file) override;

    void SaveBin(QFile &file) override;

//    QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h) override;
    void ReInitialize() override;


};

#endif // LIMAGEVZ200_H
