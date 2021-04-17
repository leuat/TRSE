#ifndef LIMAGEATARI520ST_H
#define LIMAGEATARI520ST_H

#include "limageamiga4.h"

class LImageAtari520ST : public LImageAmiga
{
public:

    LImageAtari520ST(LColorList::Type t, int type);

    virtual void InitPens() override;

    void ExportBin(QFile &file) override;

    void SaveBin(QFile &file) override;

    void LoadBin(QFile &file) override;


};

#endif // LIMAGEATARI520ST_H
