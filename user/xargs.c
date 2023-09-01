#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

char*
gets_n(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    if(c == '\n')
      break;
    buf[i++] = c;
  }
  buf[i] = '\0';
  return buf;
}

void
xargs(int argc, char *argv[])
{
  int i;
  char *uargv[MAXARG], buf[512];

  if (argc-1 > MAXARG) {
    fprintf(2, "xargs: too many argument\n");
    exit(1);
  }

  for (i = 0; i < argc-1; i++)
    uargv[i] = argv[i+1];

  while (*gets_n(buf, sizeof(buf))) {
    if (fork() == 0) {
      uargv[argc-1] = buf;
      uargv[argc] = 0;
      exec(uargv[0], uargv);
    } else {
      wait(0);
    }
  }
}

int
main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(2, "usage: xargs command\n");
    exit(1);
  }

  xargs(argc, argv);
  exit(0);
}
