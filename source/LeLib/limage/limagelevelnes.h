#ifndef LIMAGELEVELNES_H
#define LIMAGELEVELNES_H

#include "imageleveleditor.h"


class LImageLevelNES : public ImageLevelEditor
{
public:
    LImageLevelNES(LColorList::Type t);

    void SetColor(uchar col, uchar idx) override;

};

#endif // LIMAGELEVELNES_H
