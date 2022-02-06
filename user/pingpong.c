#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p1[2], p2[2];
  pipe(p1);
  pipe(p2);

  char buf[10];

  if(fork() == 0) {
    int from_parent = dup(p1[0]);
    int to_parent = dup(p2[1]);
    close(p1[0]);
    close(p1[1]);
    read(from_parent, &buf, 1);
    fprintf(1, "%d: received ping\n", getpid());
    write(to_parent, " ", 1); 
  } else {
    int to_child = dup(p1[1]);
    int from_child = dup(p2[0]); 
    write(to_child, " ", 1);
    read(from_child, &buf, 1);
    fprintf(1, "%d: received pong\n", getpid());
  }

  exit(0);
}
