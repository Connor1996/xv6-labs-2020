#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if(argc <= 1){
    fprintf(1, "should specify sleep time");
    exit(0);
  }

  int time = atoi(argv[1]);
  exit(sleep(time));
}
