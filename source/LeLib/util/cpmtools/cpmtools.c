/* #includes */ /*{{{C}}}*//*{{{*/
//#include "config.h"

#include <sys/stat.h>
#include <sys/types.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <fcntl.h>
//#include "getopt_.h"
#include "cpmfs.h"
/*}}}*/

const char cmd[]="cpmcp";
static int text=0;
static int preserve=0;
static int xit = 0;
static int optind = 0;
/**
 * Return the user number.
 * @param s CP/M filename in 0[0]:aaaaaaaa.bbb format.
 * @returns The user number or -1 for no match.
 */
static int userNumber(const char *s) /*{{{*/
{
  if (isdigit(*s) && *(s+1)==':') return (*s-'0');
  if (isdigit(*s) && isdigit(*(s+1)) && *(s+2)==':') return (10*(*s-'0')+(*(s+1)-'0'));
  return -1;
}
/*}}}*/

/**
 * Copy one file from CP/M to UNIX.
 * @param root The inode for the root directory.
 * @param src  The CP/M filename in 00aaaaaaaabbb format.
 * @param dest The UNIX filename.
 * @returns 0 for success, 1 for error.
 */
static int cpmToUnix(const struct cpmInode *root, const char *src, const char *dest) /*{{{*/
{
  struct cpmInode ino;
  int exitcode=0;

  if (cpmNamei(root,src,&ino)==-1) { fprintf(stderr,"%s: can not open `%s': %s\n",cmd,src,boo); exitcode=1; }
  else
  {
    struct cpmFile file;
    FILE *ufp;

    cpmOpen(&ino,&file,O_RDONLY);
    if ((ufp=fopen(dest,text ? "w" : "wb"))==(FILE*)0) { fprintf(stderr,"%s: can not create %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; }
    else
    {
      int crpending=0;
      int ohno=0;
      ssize_t res;
      char buf[4096];

      while ((res=cpmRead(&file,buf,sizeof(buf)))>0)
      {
        int j;

        for (j=0; j<res; ++j)
        {
          if (text)
          {
            if (buf[j]=='\032') goto endwhile;
            if (crpending)
            {
              if (buf[j]=='\n') 
              {
                if (putc('\n',ufp)==EOF) { fprintf(stderr,"%s: can not write %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; goto endwhile; }
                crpending=0;
              }
              else if (putc('\r',ufp)==EOF) { fprintf(stderr,"%s: can not write %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; goto endwhile; }
              crpending=(buf[j]=='\r');
            }
            else
            {
              if (buf[j]=='\r') crpending=1;
              else if (putc(buf[j],ufp)==EOF) { fprintf(stderr,"%s: can not write %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; goto endwhile; }
            }
          }
          else if (putc(buf[j],ufp)==EOF) { fprintf(stderr,"%s: can not write %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; goto endwhile; }
        }
      }
      endwhile:
//      if (res==-1 && !ohno) { fprintf(stderr,"%s: can not read %s (%s)\n",cmd,src,boo); exitcode=1; ohno=1; }
  //    if (fclose(ufp)==EOF && !ohno) { fprintf(stderr,"%s: can not close %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; }
      if (preserve && !ohno && (ino.atime || ino.mtime))
      {
    /*    struct utimbuf ut;

        if (ino.atime) ut.actime=ino.atime; else time(&ut.actime);
        if (ino.mtime) ut.modtime=ino.mtime; else time(&ut.modtime);
        if (utime(dest,&ut)==-1) { fprintf(stderr,"%s: can change timestamps of %s: %s\n",cmd,dest,strerror(errno)); exitcode=1; ohno=1; }
        */
      }
    }
    cpmClose(&file);
  }
  return exitcode;
}
/*}}}*/

static void usage(void) /*{{{*/
{
  fprintf(stderr,"Usage: %s [-f format] [-p] [-t] image user:file file\n",cmd);
  fprintf(stderr,"       %s [-f format] [-p] [-t] image user:file ... directory\n",cmd);
  fprintf(stderr,"       %s [-f format] [-p] [-t] image file user:file\n",cmd);
  fprintf(stderr,"       %s [-f format] [-p] [-t] image file ... user:\n",cmd);
    xit = 1;
}
/*}}}*/

int main_cpmtools(int argc, char *argv[])
{
  /* variables */ /*{{{*/
  const char *err = NULL;
  const char *image = NULL;
  const char *format = NULL;
  const char *devopts=NULL;
  int uppercase=0;
  int c=0,readcpm=-1,todir=-1;
  struct cpmInode root;
  struct cpmSuperBlock super;
  int exitcode=0;
  int gargc;
  char **gargv;
  /*}}}*/

  optind = 1;
//  opterr = 0;
//  optopt = 0;

  //cmd[]="cpmcp";
  text=0;
  xit = 0;
  preserve=0;


//  printf("here 9 \n");
  /* parse options */ /*{{{*/
//  if (!(format=getenv("CPMTOOLSFMT"))) format=FORMAT;
/*  while ((c=getopt(argc,argv,"T:f:ptuh?"))!=EOF) switch(c)
  {
    case 'T': devopts=optarg; break;
    case 'f': format=optarg; break;
    case 'p': preserve=1; break;
    case 't': text=1; break;
    case 'u': uppercase=1; break;
    case 'h':
    case '?': usage(); break;
  }*/
  format = "tim011";
  /*}}}*/
  /* parse arguments */ /*{{{*/
  if ((optind+2)>=argc) usage();
  image=argv[optind++];
//return 0;
  if (userNumber(argv[optind])>=0) /* cpm -> unix? */ /*{{{*/
  {
    int i;
    struct stat statbuf;

    for (i=optind; i<(argc-1); ++i) if (userNumber(argv[i])==-1) usage();
    todir=((argc-optind)>2);
    if (stat(argv[argc-1],&statbuf)==-1) { if (todir) usage(); }
    else if (S_ISDIR(statbuf.st_mode)) todir=1; else if (todir) usage();
    readcpm=1;
  }
  /*}}}*/
  else if (userNumber(argv[argc-1])>=0) /* unix -> cpm */ /*{{{*/
  {
    int i = 0;

    todir=0;
    for (i=optind; i<(argc-1); ++i) if (userNumber(argv[i])>=0) usage();
    if ((argc-optind)>2 && *(strchr(argv[argc-1],':')+1)!='\0') usage();
    if (*(strchr(argv[argc-1],':')+1)=='\0') todir=1;
    readcpm=0;
  }
  /*}}}*/
  else usage();
/*  if (xit==1) {
      printf("returning\n");
      return 0;
  }*/
  /*}}}*/
  /* open image file */ /*{{{*/
  if ((err=Device_open(&super.dev,image,readcpm ? O_RDONLY : O_RDWR, devopts)))
  {
    fprintf(stderr,"%s: cannot open %s (%s)\n",cmd,image,err);
  }
  if (cpmReadSuper(&super,&root,format,uppercase)==-1)
  {
    fprintf(stderr,"%s: cannot read superblock (%s)\n",cmd,boo);
  }
  /*}}}*/
//  return 0;
  if (readcpm) /* copy from CP/M to UNIX */ /*{{{*/
  {
    int i;
    char *last=argv[argc-1];
    
    cpmglob(optind,argc-1,argv,&root,&gargc,&gargv);
    /* trying to copy multiple files to a file? */
    if (gargc>1 && !todir) usage();
    for (i=0; i<gargc; ++i)
    {
      char dest[1024];

      if (todir)
      {
        char *translate;

        strcpy(dest,last);
        strcat(dest,"/");
        translate=dest+strlen(dest);
        strcat(dest,gargv[i]+2);
        while ((translate=strchr(translate,'/'))) *translate=',';
      }
      else strcpy(dest,last);
      if (cpmToUnix(&root,gargv[i],dest)) exitcode=1;
    }
  }
  /*}}}*/
  else /* copy from UNIX to CP/M */ /*{{{*/
  {
    int i;

    for (i=optind; i<(argc-1); ++i)
    {
      /* variables */ /*{{{*/
      char *dest=(char*)0;
      FILE *ufp;
      /*}}}*/

      if ((ufp=fopen(argv[i],"rb"))==(FILE*)0) /* cry a little */ /*{{{*/
      {
        fprintf(stderr,"%s: can not open %s: %s\n",cmd,argv[i],strerror(errno));
        exitcode=1;
      }
      /*}}}*/
      else
      {
        struct cpmInode ino;
        char cpmname[2+8+1+3+1]; /* 00foobarxy.zzy\0 */
        char *translate;
        struct stat st;

        stat(argv[i],&st);

        if (todir)
        {
          if ((dest=strrchr(argv[i],'/'))!=(char*)0) ++dest; else dest=argv[i];
          snprintf(cpmname,sizeof(cpmname),"%02d%s",userNumber(argv[argc-1]),dest);
        }
        else
        {
          snprintf(cpmname,sizeof(cpmname),"%02d%s",userNumber(argv[argc-1]),strchr(argv[argc-1],':')+1);
        }

        translate=cpmname;
        while ((translate=strchr(translate,','))) *translate='/';

        if (cpmCreat(&root,cpmname,&ino,0666)==-1) /* just cry */ /*{{{*/
        {
          fprintf(stderr,"%s: can not create %s: %s\n",cmd,cpmname,boo);
          exitcode=1;
        }
        /*}}}*/
        else
        {
          struct cpmFile file;
          int ohno=0;
          char buf[4096+1];
          cpmOpen(&ino,&file,O_WRONLY);
          do
          {
            ssize_t j;

            for (j=0; j<((ssize_t)sizeof(buf)/2) && (c=getc(ufp))!=EOF; ++j)
            {
              if (text && c=='\n') buf[j++]='\r';
              buf[j]=c;
            }
            if (text && c==EOF) buf[j++]='\032';
            if (cpmWrite(&file,buf,j)!=j)
            {
              fprintf(stderr,"%s: can not write %s: %s\n",cmd,dest,boo);
              ohno=1;
              exitcode=1;
              break;
            }
          } while (c!=EOF);
          if (cpmClose(&file)==EOF && !ohno) /* I just can't hold back the tears */ /*{{{*/
          {
            fprintf(stderr,"%s: can not close %s: %s\n",cmd,dest,boo);
            exitcode=1;
          }
          /*}}}*/
          if (preserve && !ohno)
          {
/*            struct utimbuf times;
            times.actime=st.st_atime;
            times.modtime=st.st_mtime;
            cpmUtime(&ino,&times);
            */
          }
        }
        if (fclose(ufp)==EOF)
        {
          fprintf(stderr,"%s: can not close %s: %s\n",cmd,dest,strerror(errno));
          exitcode=1;
        }
      }
    }
  }
  /*}}}*/
  if (cpmUmount(&super)==-1)
  {
    fprintf(stderr,"%s: can not umount device: %s\n",cmd,boo);
    exitcode=1;
  }
//  exit(exitcode);
}
