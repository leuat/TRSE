#ifndef TIKIDISK_H
#define TIKIDISK_H

typedef unsigned char byte;
typedef unsigned short word;
typedef short boolean;

/*
class TikiDisk
{
public:
    TikiDisk();
};
*/
extern "C" {
    void addTikiFile (char *efn, unsigned char user);
    void InitTikiDisk(char* file);
    void SaveTikiDisk(char* file);

}

#endif // TIKIDISK_H
