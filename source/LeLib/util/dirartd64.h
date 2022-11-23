#ifndef DIRARTD64_H
#define DIRARTD64_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <QString>


class DirArtD64
{
public:
    DirArtD64();

#define T18_OFFS 0x16500
#define BAM_TRACK(t) disk + T18_OFFS + (t*4)
#define BAM_OFFS(t,s) BAM_TRACK(t) + (s/8) + 1

#define SECTOR(s) disk + T18_OFFS + (s*256)

    int stoi(char* s);



    uint8_t *disk;


    void err(QString s);

    int is_sector_free(int t, int s);

    void take_sector(int t, int s);

    void Write(int argc, char* argv[]);

};

#endif // DIRARTD64_H
