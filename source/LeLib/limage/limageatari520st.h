#ifndef LIMAGEATARI520ST_H
#define LIMAGEATARI520ST_H

#include "limageamiga4.h"

class LImageAtari520ST : public LImageAmiga
{
public:
// TODO(ColinPitrat): Replace by an inline function and deduplicate with the other different CHECK_BIT in source/LeLib/limage/pixelchar.h
#ifndef CHECK_BIT
#define CHECK_BIT(var,pos) (((var) & (1<<(pos)))>>pos)
#endif

    LImageAtari520ST(LColorList::Type t, int type);

    virtual void InitPens() override;

    void ExportBin(QFile &file) override;

    void SaveBin(QFile &file) override;

    void LoadBin(QFile &file) override;

    QStringList SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int x, int y, int w, int h, QString pparam) override;

    void PrintShift(QString name, QString &s, int x, int y, int w, int h, int shift);

};

#endif // LIMAGEATARI520ST_H
