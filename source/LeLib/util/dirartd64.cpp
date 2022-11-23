#include "dirartd64.h"

DirArtD64::DirArtD64()
{

}




void DirArtD64::Write(int argc, char* argv[])
{
    char* srcname = NULL;
    char* diskname = NULL;
    char* outname = NULL;

    int srcbin = 0;
    int srcpet = 0;

    int needhelp = 0;
    if (argc < 4) needhelp++;
    for (int i = 1; i < argc; i++)
    {
        char* arg = argv[i];
        if (strlen(arg) == 2 && arg[0] == '-')
        {
            switch (arg[1])
            {
                case 't':
                    srcbin = 0;
                    break;
                case 'b':
                    srcbin = 1;
                    break;
                case 's':
                    srcpet = 0;
                    break;
                case 'p':
                    srcpet = 1;
                    break;
                case '?':
                    needhelp++;
                    break;
                default:
                    printf("Unrecognized option -%c\n", arg[1]);
//                    exit(EXIT_FAILURE);
            }
        }
        else if (!strcmp(arg, "--help"))
            needhelp++;
        else
        {
            if (outname)
                err("Too many arguments");
            else if (diskname) outname = arg;
            else if (srcname) diskname = arg;
            else srcname = arg;
        }
    }

    uint8_t *art = NULL;
    size_t artlen = 0;

    FILE *f = fopen(srcname, "rb");
    if (!f)
    {
        perror("Couldn't open source file");
//        exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    size_t fsize = ftell(f);
    char *fbuf = (char*)malloc(fsize+1);
    rewind(f);
    fread(fbuf, 1, fsize, f);
    fclose(f);

    if (srcbin)
    {
        art = (uint8_t*)malloc(fsize);
        artlen = fsize;
        memcpy(art, fbuf, fsize);
    }
    else
    {
        fbuf[fsize] = '\0';
        char *tok = strtok(fbuf, " ,\n\f\r\t\v");
        while (tok)
        {
            int i = stoi(tok);
            if (i >= 0 && i < 256)
            {
                art = (uint8_t*)realloc(art, artlen+1);
                art[artlen++] = i;
            }
            tok = strtok(NULL, " ,\n\f\r\t\v");
        }

    }

    free(fbuf);



    if (artlen % 0x10)
        err("Art length must be a multiple of 16");

    int illegal = 0;
    for (int i = 0; i < artlen; i++)
    {
        uint8_t c = art[i];
        if (!srcpet)
        {
            if (c == 0x80 || c == 0x8d || c == 0x94 || c == 0xcd || c == 0x60 ||
                (c >= 0xa0 && c < 0xc0) ||
                (c >= 0xe0) )
            {
                throw QString("Art contains illegal screencode $"+QString::number(c,16)+" in filename "+QString::number(i/16+1));
//                throw QString("Art contains illegal screencode $%02x in filename %i, char %i\n", c, i/16+1, i%16+1);
                illegal++;
            }
            if (c < 0x20) c += 0x40;
            else if (c < 0x40)  /* nothing */ ;
            else if (c == 0x5e) c = 0xff;
            else if (c < 0x60) c += 0x80;
            else if (c < 0x80) c += 0x40;
            else if (c < 0xa0) c += 0x80;
            else if (c < 0xe0) c += 0xc0;

            art[i] = c;
        }
        else if (c == 0x00 || c == 0x0d || c == 0x14 || c == 0x8d || c == 0xa0)
        {
            printf("Art contains illegal petscii code $%02x in filename %i, char %i\n", c, i/16+1, i%16+1);
            illegal++;
        }
    }
//    if (illegal) exit(EXIT_FAILURE);


    f = fopen(diskname, "rb");
    if (!f)
    {
        perror("Couldn't open disk file");
  //      exit(EXIT_FAILURE);
    }
    fseek(f, 0, SEEK_END);
    fsize = ftell(f);
    if (fsize != 174848 && fsize != 175531 && fsize != 176608 && fsize != 197376)
    {
        fclose(f);
        err("Disk file is not a valid D64");
    }
    rewind(f);
    disk = (uint8_t*)malloc(fsize);
    fread(disk, 1, fsize, f);
    fclose(f);

    uint8_t *file = SECTOR(1);
    int sector = 1;
    int makenew = 0;
    for (int artindex = 0; artindex < artlen; artindex += 16)
    {
        if (!makenew && !(file[2] & 0x80))
            makenew++;
        if (makenew)
        {
            memset(file, 0, 0x20);
            file[2] = 0x80;
        }
        memcpy(file+5, art+artindex, 0x10);

        if (((artindex & 0x70) == 0x70) && (artindex < artlen-0x10)) /* next sector? */
        {
            if (file[0]) /* t/s link exists */
            {
                if (file[0] != 18 || file[1] >= 19)
                {
                    printf("Directory contains invalid T/S link to %i/%i\n", file[0], file[1]);
//                    exit(EXIT_FAILURE);
                }
                sector = file[1];

                file = SECTOR(sector);
            }
            else  /* allocate new */
            {
                int nextsector = -1;
                for (int s = (sector + 3) % 19; s != sector; s = (s+3) % 19)
                {
                    if (is_sector_free(18, s))
                    {
                        nextsector = s;
                        break;
                    }
                }
                if (nextsector < 0)
                    err("Not enough free sectors on track 18");

                file = SECTOR(sector);
                file[0] = 18;
                file[1] = nextsector;

                sector = nextsector;
                file = SECTOR(sector);
                memset(file, 0, 0x100);

                take_sector(18, sector);
            }
        }
        else
        {
            file += 0x20;
        }
    }

    f = fopen(outname, "wb");
    fwrite(disk, 1, fsize, f);
    fclose(f);


}

void DirArtD64::take_sector(int t, int s)
{
    uint8_t *p = BAM_OFFS(t,s);
    *p = *p & (0xff ^ (1 << (s&7)));
    p = BAM_TRACK(t);
    *p = *p - 1;
}

int DirArtD64::is_sector_free(int t, int s)
{
    return (*BAM_OFFS(t,s)) & (1 << (s&7));
}

void DirArtD64::err(QString s)
{
    throw QString(s);
//    puts(s);
  //  exit(EXIT_FAILURE);
}

int DirArtD64::stoi(char *s)
{
    int hex = 0;
    if (s[0] == '0' && s[1] == 'x')
    {
        hex++;
        s += 2;
    }
    else if (s[0] == '$')
    {
        hex++;
        s++;
    }
    int conv = 0;
    int place = 1;
    for (char *p = s+strlen(s)-1; p >= s; p--)
    {
        char c = tolower(*p);
        if (c >= '0' && c <= '9')
        {
            conv += (c - '0') * place;
            goto good_char;
        }
        if (hex && (c >= 'a' && c <= 'f'))
        {
            conv += (c - 'a' + 0x0a) * place;
            goto good_char;
        }
        return -1;
good_char:
        place *= hex ? 0x10 : 10;
    }
    return conv;
}
