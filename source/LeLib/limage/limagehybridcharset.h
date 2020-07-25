#ifndef LIMAGEHYBRIDCHARSET_H
#define LIMAGEHYBRIDCHARSET_H

#include "charsetimage.h"

class LImageHybridCharset : public CharsetImage
{
public:
    LImageHybridCharset(LColorList::Type t);

    unsigned int getPixel(int x, int y) override;
    void setPixel(int x, int y, unsigned int color) override;

};

#endif // LIMAGEHYBRIDCHARSET_H
