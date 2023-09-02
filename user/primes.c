#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
primes(int p)
{
  int i, prime, np[2];

  read(p, &prime, sizeof(int));
  printf("prime %d\n", prime);
  
  if (!read(p, &i, sizeof(int))) {
    close(p);
    return;
  }
  
  pipe(np);
  if (fork() == 0) {
    close(p);
    close(np[1]);
    primes(np[0]);
  } else {
    close(np[0]);
    do {
      if (i % prime != 0)
        write(np[1], &i, sizeof(int));
    } while (read(p, &i, sizeof(int)));
    close(p);
    close(np[1]);
    wait(0);
  }
}

int
main(int argc, char *argv[])
{
  int i, p[2];

  pipe(p);
  for (i = 2; i <= 35; i++)
    write(p[1], &i, sizeof(int));
  close(p[1]);
  primes(p[0]);
  exit(0);
}
