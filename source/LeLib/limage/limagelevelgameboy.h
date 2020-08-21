#ifndef LIMAGELEVELGAMEBOY_H
#define LIMAGELEVELGAMEBOY_H

#include "limagelevelnes.h"


class LImageLevelGameboy : public LImageLevelNES
{
public:
    LImageLevelGameboy(LColorList::Type t) : LImageLevelNES(t) {
        m_type = LImage::Type::LevelEditorGameboy;

    }

    void ExportBin(QFile &file) override;
    void SetPalette(int pal) override;

};

#endif // LIMAGELEVELGAMEBOY_H
