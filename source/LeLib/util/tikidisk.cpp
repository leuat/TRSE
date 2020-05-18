#include "tikidisk.h"

extern "C" {
    void addTikiFile (char *efn, unsigned char user);
}

#include <stdio.h>
  #include <ctype.h>
  #include <string.h>
  #include <stdlib.h>


  #ifndef TRUE
  #define TRUE   ~0
  #endif
  #ifndef FALSE
  #define FALSE   0
  #endif
  #ifndef NULL
  #define NULL    0
  #endif

  #define DISK90K   1*40*18*128
  #define DISK200K  1*40*10*512
  #define DISK400K  2*40*10*512
  #define DISK800K  2*80*10*512

  /* protos */
//  int main (int argc, char *argv[]);

  void saveFile (byte *entry);
  void addTikiFile (char *efn, byte user);
  void delFile (char *ffn, byte user);

  byte *getEntry (int entryNumber);
  byte *getExtent (byte *entry, int extentNumber);
  byte *getMatch (char *ffn, byte user);
  byte *getSector (int sectorNumber);

  int getExtentNumber (byte *entry);
  void setExtentNumber (byte *entry, int extentNumber);
  int getBytesInEntry (byte *entry);
  void setBytesInEntry (byte *entry, int bytesInExtent);
  int getBlockNumber (byte *entry, int alNumber);
  void setBlockNumber (byte *entry, int alNumber, int blockNumber);

  void getFilename (char *filename, byte *entry);
  int getFilesize (byte *entry);
  byte getUsernumber (byte *entry);
  void getAttribs (char *attribs, byte *entry);
  void setNameAndUser (byte *ffnEntry, char *ffn, byte user);

  void buildBAM (void);
  int allocateBlock (void);

  boolean entryNotDeleted (byte *entry);
  void deleteEntry (byte *entry);
  boolean entryEquals (byte *ffnEntry, byte *entry);
  char legalChar (char c);
  char *stripPath (char *ffn);

  /* globals */
  byte diskImage[DISK800K]; /* her legges diskettfil */
  boolean *bam = NULL;      /* Block Availability Map */

  int numberOfEntries;      /* antall kataloginnganger */
  int systemSectors;        /* antall sektorer med bootkode */
  int sectorSize;           /* bytes pr. sektor */
  int *convertSectorNumber; /* tabell som mapper sektornummer */
  int sectorsPerTrack;      /* antall sektorer pr. spor */
  int exm;                  /* antall bytes pr. AL */
  int bytesPerAL;           /* blokkstørrelse */
  int blockSize;            /* bytes pr. blokk */
  int sectorsPerBlock;      /* sektorer pr. blokk */
  int numberOfBlocks;       /* antall blokker (untatt systemsektorer) */
  int directoryBlocks;      /* antall blokker brukt av kataloginnganger */
  int numberOfALs;          /* antall ALs pr. kataloginngang */

  int convertSectorNumber18[18] = {0, 5, 10, 15, 2, 7, 12, 17, 4, 9, 14, 1, 6, 11, 16, 3, 8, 13};
  int convertSectorNumber10[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int size;

  /* Trekker ut fil fra diskettfil og lagrer på disk
   *
   * entry: en ekstent til filen som skal lagres
   */
  void SaveTikiDisk(char* file) {
      FILE *diskImageFp;

      if ((diskImageFp = fopen (file, "wb"))) {
        fwrite (diskImage, 1, size, diskImageFp);
        fclose (diskImageFp);
      }
   //   else exit(1);

  }


  void InitTikiDisk(char* file) {
        FILE *diskImageFp;
        byte user;

        if (!(diskImageFp = fopen (file, "rb"))) {
 //          exit(1);
        }
        size = fread (diskImage, 1, DISK800K, diskImageFp);
        fclose (diskImageFp);

      /* sett diskparametre */
      switch (size) {
        case DISK90K:
          convertSectorNumber = convertSectorNumber18;
          numberOfEntries = 32;
          systemSectors = 3 * 18;
          sectorSize = 128;
          sectorsPerTrack = 18;
          exm = 0;
          bytesPerAL = 1;
          blockSize = 1024;
          numberOfBlocks = 83;
          directoryBlocks = 1;
          break;
        case DISK200K:
          convertSectorNumber = convertSectorNumber10;
          numberOfEntries = 64;
          systemSectors = 20;
          sectorSize = 512;
          sectorsPerTrack = 10;
          exm = 0;
          bytesPerAL = 1;
          blockSize = 1024;
          numberOfBlocks = 190;
          directoryBlocks = 2;
          break;
        case DISK400K:
          convertSectorNumber = convertSectorNumber10;
          numberOfEntries = 128;
          systemSectors = 20;
          sectorSize = 512;
          sectorsPerTrack = 10;
          exm = 1;
          bytesPerAL = 1;
          blockSize = 2048;
          numberOfBlocks = 195;
          directoryBlocks = 2;
          break;
        case DISK800K:
          convertSectorNumber = convertSectorNumber10;
          numberOfEntries = 128;
          systemSectors = 20;
          sectorSize = 512;
          sectorsPerTrack = 10;
          exm = 0;
          bytesPerAL = 2;
          blockSize = 2048;
          numberOfBlocks = 395;
          directoryBlocks = 2;
          break;
        default:
          fprintf (stderr, "Ikke en gyldig diskfil\n");
          exit (1);
      }
      sectorsPerBlock = blockSize / sectorSize;
      numberOfALs = 16 / bytesPerAL;

}

  void saveFile (byte *entry) {
    char filename[13];
    FILE *fp;

    getFilename (filename, entry);

    if ((fp = fopen (filename, "wb"))) {
      int extentNumber = 0;

      printf ("Henter ut %02d:%s...\n", getUsernumber (entry), filename);

      /* gå gjennom alle extenter */
      while ((entry = getExtent (entry, extentNumber++))) {
        boolean done = FALSE;
        int blockNumber;
        int alNumber = 0;
        int bytesRemaining = getBytesInEntry (entry);

        /* gå gjennom alle blokker */
        while (!done && (alNumber < numberOfALs)) {
          if ((blockNumber = getBlockNumber (entry, alNumber))) {
            int i;

            /* lagre sektorer i blokk */
            for (i = 0; i < sectorsPerBlock; i++) {
              int sectorNumber = ((blockNumber * blockSize) / sectorSize) + systemSectors + i;

              if (bytesRemaining) {
                int bytesToWrite = (bytesRemaining < sectorSize ? bytesRemaining : sectorSize);

                fwrite (getSector (sectorNumber), 1, bytesToWrite, fp);
                bytesRemaining -= bytesToWrite;
              } else {
                done = TRUE;
              }
            }
          }
          alNumber++;
        }
      }
      fclose (fp);
    }
  }

  /* Henter inn fil fra disk og legger inn i diskettfil
   *
   * efn: entydig filnavn til fil som skal legges til
   * user: entydig brukernummer som fil skal lagres under
   */
  void addTikiFile (char *efn, byte user) {
    FILE *fp;
    byte *entry = NULL;
    int extentNumber = 0;
    int alNumber = 0;
    int sectorNumber = 0;
    int bytesInExtent = 0;
    byte *sectorData;
    boolean done = FALSE;

    if (!(fp = fopen (efn, "rb"))) {
        exit(1);
       // qDebug() << "Cannot open file : "<< efn;
//      fprintf (stderr, "Kan ikke åpne %s\n", efn);
      return;
    }

    if (!(sectorData = (unsigned char*)malloc (sectorSize))) {
      fprintf (stderr, "Ikke nok minne\n");
      free (bam);
      exit (1);
    }

    delFile (stripPath (efn), user);

    buildBAM();

//    printf ("Legger til %02d:%s...\n", user, efn);

    while (!done) {
      int bytesRead = fread (sectorData, 1, sectorSize, fp);

      if (bytesRead) {

        /* finn ledig kataloginngang */
        if (!(bytesInExtent % (numberOfALs * blockSize))) {
          int entryNumber = 0;

          if (entry) setBytesInEntry (entry, bytesInExtent);

          do {
            if (entryNumber >= numberOfEntries) {
              fprintf (stderr, "Ikke nok ledige kataloginnganger\n");
              free (sectorData);
              free (bam);
              exit (1);
            }
            entry = getEntry (entryNumber);
            entryNumber++;
          } while (entryNotDeleted (entry));

          memset (entry, 0, 32);
          setNameAndUser (entry, stripPath (efn), user);
          setExtentNumber (entry, extentNumber++);
          alNumber = 0;
          bytesInExtent = 0;
        }

        /* finn ledig blokk */
        if (!(bytesInExtent % blockSize)) {
          int blockNumber;

          if (!(blockNumber = allocateBlock())) {
            fprintf (stderr, "Ikke nok ledige blokker\n");
            free (sectorData);
            free (bam);
            exit (1);
          }
          setBlockNumber (entry, alNumber++, blockNumber);
          sectorNumber = ((blockNumber * blockSize) / sectorSize) + systemSectors;
        }

        /* skriv til sektor */
        memcpy (getSector (sectorNumber++), sectorData, sectorSize);
        bytesInExtent += bytesRead;

      } else {
        done = TRUE;
        setBytesInEntry (entry, bytesInExtent);
      }
    }
    free (sectorData);




  }

  /* Sletter filer fra diskettfil
   *
   * ffn: flertydig filnavn til filer som skal slettes
   * user: flertydig brukernummer til filer som skal slettes
   */
  void delFile (char *ffn, byte user) {
    byte *entry;

    if ((entry = getMatch (ffn, user))) {
      /* gå gjennom alle entries (filer) */
      do {
        int extentNumber = 0;
        byte *extent;
        char filename[13];

        getFilename (filename, entry);

        printf ("Sletter %02d:%s...\n", getUsernumber (entry), filename);

        /* slett alle extenter til filen */
        while ((extent = getExtent (entry, extentNumber++))) {
          deleteEntry (extent);
        }
      } while ((entry = getMatch (NULL, 0)));
    }
  }

  /* Gir peker til entry i diskettfil
   *
   * entryNumber: nummer på entry som skal returneres
   */
  byte *getEntry (int entryNumber) {
    int dirSector = systemSectors + ((entryNumber * 32) / sectorSize);
    int offsetInSector = (entryNumber * 32) % sectorSize;

    return getSector (dirSector) + offsetInSector;
  }

  /* Gir peker til extent i diskettfil, eller NULL derom ikke funnet
   *
   * entry: en allerede kjent extent
   * extentNumber: nummer til ønsket extent
   */
  byte *getExtent (byte *entry, int extentNumber) {
    int entryNumber;

    for (entryNumber = 0; entryNumber < numberOfEntries; entryNumber++) {
      byte *curEntry = getEntry (entryNumber);

      if (entryNotDeleted (curEntry) && entryEquals (entry, curEntry) &&
          (getExtentNumber (curEntry) == extentNumber)) {
        return curEntry;
      }
    }
    return NULL;
  }

  /* Gir peker til neste fil (extent 0) som passer, eller NULL dersom ikke flere passer
   *
   * ffn: flertydig filnavn til filer som skal finnes
   *      Angi ffn kun første gang funksjonen kalles. For å finne de resterende filer
   *      som passer, kall funksjonen med ffn = NULL
   * user: flertydig brukernummer til filer som skal finnes
   */
  byte *getMatch (char *ffn, byte user) {
    static byte ffnEntry[12];
    static int entryNumber;

    if (ffn != NULL) {
      entryNumber = 0;
      setNameAndUser (ffnEntry, ffn, user);
    }

    while (entryNumber < numberOfEntries) {
      byte *entry = getEntry (entryNumber);
      entryNumber++;
      if (entryNotDeleted (entry) && entryEquals (ffnEntry, entry) && (getExtentNumber (entry) == 0)) {
        return entry;
      }
    }
    return NULL;
  }

  /* Gir peker til stedet der data for angitt sektor befinner seg
   *
   * sectorNumber: nummer på sektor som ønskes
   */
  byte *getSector (int sectorNumber) {
    int realSectorNumber = (sectorNumber / sectorsPerTrack) * sectorsPerTrack +
      convertSectorNumber[sectorNumber % sectorsPerTrack];
    return diskImage + realSectorNumber * sectorSize;
  }

  /* Gir nummeret på extent (fra EX og S2 feltene)
   *
   * entry: kataloginngang man ønsker extentnummeret til
   */
  int getExtentNumber (byte *entry) {
    return (*(entry + 14) * 32 + *(entry + 12)) / (exm + 1);
  }

  /* Setter nummer på extent (til EX og S2 feltene)
   *
   * entry: kataloginngang man skal sette extentnummeret til
   * extentNumber: extentnummeret som skal settes
   */
  void setExtentNumber (byte *entry, int extentNumber) {
    *(entry + 14) = extentNumber / 32;
    *(entry + 12) = ((extentNumber % 32) * (exm + 1));
  }

  /* Gir antall bytes lagret i extent (fra RC og EX feltene)
   *
   * entry: extent man ønsker størrelsen til
   */
  int getBytesInEntry (byte *entry) {
    return ((*(entry + 12) & exm) * 128 + *(entry + 15)) * 128;
  }

  /* Setter antall bytes lagret i extent (til RC og EX feltene)
   *
   * entry: kataloginngang man skal sette størrelsen til
   * bytesInExtent: antall bytes som skal settes
   */
  void setBytesInEntry (byte *entry, int bytesInExtent) {
    int recordsInExtent = bytesInExtent / 128;

    if (bytesInExtent % 128) recordsInExtent++;

    if (recordsInExtent % 0x80) {
      *(entry + 15) = recordsInExtent % 0x80;
    } else {
      *(entry + 15) = 0x80;
    }
    if (recordsInExtent > 0x80) {
      *(entry + 12) += (recordsInExtent / 0x80);
    }
  }

  /* Gir nummeret på blokken som befinner seg i en AL
   *
   * entry: kataloginngang
   * alNumber: nummer på AL man ønsker blokknummeret til
   */
  int getBlockNumber (byte *entry, int alNumber) {
    int al1 = *(entry + 16 + (alNumber * bytesPerAL));
    int al2 = *(entry + 16 + (alNumber * bytesPerAL) + 1);

    return (bytesPerAL == 1 ? al1 : al1 + (al2 * 256));
  }

  /* Setter nummer på blokk i an AL
   *
   * entry: kataloginngang
   * alNumber: nummer på AL
   * blockNumber: nummer på blokk
   */
  void setBlockNumber (byte *entry, int alNumber, int blockNumber) {
    if (bytesPerAL < 2) {
      *(entry + 16 + alNumber) = blockNumber;
    } else {
      *(entry + 16 + (alNumber * 2)) = blockNumber;
      *(entry + 16 + (alNumber * 2) + 1) = blockNumber / 256;
    }
  }

  /* Kopierer filnavn fra en kataloginngang til en streng
   *
   * filename: streng som filnavn skal kopieres til
   * entry: kataloginngang man ønsker filnavnet til
   */
  void getFilename (char *filename, byte *entry) {
     int i = 0, j = 0;

     entry++;
     /* kopier filnavn */
     while ((i < 8) && (*entry != ' ')) {
        filename[i++] = legalChar (*entry++);
     }
     entry += 8 - i;
     filename[i++] = '.';
     /* kopier filtype */
     while ((j++ < 3) && (*entry != ' ')) {
        filename[i++] = legalChar ((*entry++) & 0x7f);
     }
     filename[i] = '\0';
  }

  /* Gir størrelsen på en fil
   *
   * entry: en extent til fila man ønsker størrelsen til
   */
  int getFilesize (byte *entry) {
    int filesize = 0;
    boolean done = FALSE;
    int extentNumber = 0;

    while (!done) {
      if (!(entry = getExtent (entry, extentNumber++))) {
        done = TRUE;
      } else {
        filesize += getBytesInEntry (entry);
      }
    }
    return filesize;
  }

  /* Gir brukernummeret til en kataloginngang
   *
   * entry: kataloginngang man ønsker brukernummeret til
   */
  byte getUsernumber (byte *entry) {
    return *entry;
  }

  /* Lager en streng som representerer attributtene til en kataloginngang
   *
   * attribs: streng som attributtinfo skal skrives til
   * entry: kataloginngang man ønsker attributtinfo til
   */
  void getAttribs (char *attribs, byte *entry) {
    *(attribs + 0) = (*(entry +  9)) & 0x80 ? 'l' : '-';
    *(attribs + 1) = (*(entry + 10)) & 0x80 ? 's' : '-';
    *(attribs + 2) = (*(entry + 11)) & 0x80 ? 'a' : '-';
    *(attribs + 3) = '\0';
  }

  /* Kopierer filnavn og brukernummer inn i en kataloginngang
   *
   * ffnEntry: kataloginngang man skal skrive til
   * ffn: flertydig filnavn som skal skrives fra
   * user: flertydig brukernummer som skal skrives fra
   */
  void setNameAndUser (byte *ffnEntry, char *ffn, byte user) {
    int i;

    /* kopier brukernummer */
    *ffnEntry++ = user;

    /* kopier filnavn */
    for (i = 0; i < 8; i++, ffnEntry++) {
      if (*ffn == '*') {
        *ffnEntry = (byte)'?';
      } else if (*ffn == '?') {
        *ffnEntry = (byte)'?';
        ffn++;
      } else if (*ffn == '.') {
        *ffnEntry = (byte)' ';
      } else if (*ffn == '\0') {
        *ffnEntry = (byte)' ';
      } else {
        *ffnEntry = (byte)legalChar (*ffn);
        ffn++;
      }
    }

    /* finn filtype */
    if (strchr (ffn, '.')) {
      ffn = strchr (ffn, '.') + 1;

      /* kopier filtype */
      for (i = 0; i < 3; i++) {
        if (*ffn == '*') {
          *ffnEntry++ = (byte)'?';
        } else if (*ffn == '?') {
          *ffnEntry++ = (byte)'?';
          ffn++;
        } else if (*ffn == '.') {
          ffn++;
        } else if (*ffn == '\0') {
          *ffnEntry++ = (byte)' ';
        } else {
          *ffnEntry++ = (byte)legalChar (*ffn);
          ffn++;
        }
      }
    }
  }

  /* Bygger Block Availability Map for diskettfil
   */
  void buildBAM (void) {
    int entryNumber;

    free (bam);
    if (!(bam = (boolean*)malloc (sizeof (boolean) * numberOfBlocks))) {
      fprintf (stderr, "Ikke nok minne\n");
      exit (1);
    }

    memset (bam, 0, sizeof (boolean) * numberOfBlocks);

    { /* merk katalogblokker som opptatte */
      int blockNumber;

      for (blockNumber = 0; blockNumber < directoryBlocks; blockNumber++) {
        *(bam + blockNumber) = TRUE;
      }
    }

    /* gå gjennom alle kataloginnganger */
    for (entryNumber = 0; entryNumber < numberOfEntries; entryNumber++) {
      byte *entry = getEntry (entryNumber);

      if (entryNotDeleted (entry)) {
        int blockNumber;
        int alNumber = 0;

        /* merk alle blokker som denne kataloginngangen bruker som opptatt */
        while ((alNumber < numberOfALs) && (blockNumber = getBlockNumber (entry, alNumber))) {
          *(bam + blockNumber) = TRUE;
          alNumber++;
        }
      }
    }
  }

  /* Allokerer en blokk og gir nummeret på den nye blokken
   */
  int allocateBlock (void) {
    int blockNumber;

    for (blockNumber = 0; blockNumber < numberOfBlocks; blockNumber++) {
      if (!*(bam + blockNumber)) {
        *(bam + blockNumber) = TRUE;
        return blockNumber;
      }
    }
    return 0;
  }

  /* Gir slettet-status til en kataloginngang
   * entry: kataloginngang
   */
  boolean entryNotDeleted (byte *entry) {
    return (*entry < 32);
  }

  /* Sletter en kataloginngang
   * entry: kataloginngang
   */
  void deleteEntry (byte *entry) {
    *entry = 0xe5;
  }

  /* Sjekker om to kataloginnganger har samme navn og brukernummer
   *
   * ffnEntry: kataloginngang med flertydig navn og brukernummer
   * entry: kataloginngang som ffnEntry skal sjekkes mot
   */
  boolean entryEquals (byte *ffnEntry, byte *entry) {
    if (((*(ffnEntry + 0) == '?') || (*(ffnEntry + 0) == *(entry + 0))) &&

        ((*(ffnEntry + 1) == '?') || (*(ffnEntry + 1) == *(entry + 1))) &&
        ((*(ffnEntry + 2) == '?') || (*(ffnEntry + 2) == *(entry + 2))) &&
        ((*(ffnEntry + 3) == '?') || (*(ffnEntry + 3) == *(entry + 3))) &&
        ((*(ffnEntry + 4) == '?') || (*(ffnEntry + 4) == *(entry + 4))) &&
        ((*(ffnEntry + 5) == '?') || (*(ffnEntry + 5) == *(entry + 5))) &&
        ((*(ffnEntry + 6) == '?') || (*(ffnEntry + 6) == *(entry + 6))) &&
        ((*(ffnEntry + 7) == '?') || (*(ffnEntry + 7) == *(entry + 7))) &&
        ((*(ffnEntry + 8) == '?') || (*(ffnEntry + 8) == *(entry + 8))) &&

        ((*(ffnEntry +  9) == '?') || (*(ffnEntry +  9) == *(entry +  9))) &&
        ((*(ffnEntry + 10) == '?') || (*(ffnEntry + 10) == *(entry + 10))) &&
        ((*(ffnEntry + 11) == '?') || (*(ffnEntry + 11) == *(entry + 11)))) {
      return TRUE;
    }
    return FALSE;
  }

  /* Konverterer et tegn til et tegn som kan brukes i alle filnavn på så godt
   * som alle operativsystem
   *
   * c: tegn som skal konverteres
   */
  char legalChar (char c) {
     const char *legalChars = "_^$~!#&-{}@'";

     if (!strchr (legalChars, c)) {
        if ((c < 48) || (c > 57 && c < 65) || (c > 90 && c < 97) || (c >122)) {
           return '#';
        }
     }
     return toupper (c);
  }

  /* Gir en peker til første tegn i filnavnet (etter evt. sti)
   *
   * ffn: filnavn med evt. sti
   */
  char *stripPath (char *ffn) {
    char *ptr;

    if (!(ptr = strrchr (ffn, '/'))) {
      if (!(ptr = strrchr (ffn, '\\'))) {
        if (!(ptr = strrchr (ffn, ':'))) {
          ptr = ffn - 1;
        }
      }
    }
    return ptr + 1;
  }
