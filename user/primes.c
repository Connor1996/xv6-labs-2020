#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int do_check(int from) {
  int p;
  if (read(from, &p, sizeof(p)) == 0) return 0;
  fprintf(1, "prime %d\n", p);

  int pip[2];
  int child = -1;
  while(1) {
    int n;
    if (read(from, &n, sizeof(n)) == 0) {
      int ret = 0;
      if (child != -1) {
        close(pip[1]);
        wait(&ret);
      }
      return ret;
    };

    if (n % p != 0) {
      if (child == -1) {
        pipe(pip);
        child = fork();
        if (child != 0) {
          close(pip[1]);
          return do_check(pip[0]);
        } else {
          close(pip[0]);
        }
      } 
      write(pip[1], &n, sizeof(n));
    }
  }
}

int
main(int argc, char *argv[])
{
  int p[2];
  pipe(p);

  int ret;
  if(fork() != 0) {
    int to = dup(p[1]);
    close(0);
    close(p[0]);
    close(p[1]);
    for (int i = 2; i <= 35; i++) {
      write(to, &i, sizeof(i));
    }
    close(to);
    wait(&ret);
  } else {
    int from = dup(p[0]);
    close(0);
    close(p[0]);
    close(p[1]);
    ret = do_check(from);
  }

  exit(ret);
}
