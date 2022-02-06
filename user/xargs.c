#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char* new_argv[MAXARG];
  for (int i = 0; i < argc - 1; i++) {
    new_argv[i] = argv[i + 1];
  }
  char buf[1024];
  int buf_i = 0;
  int arg_i = argc - 1;
  while (1) {
    int count = read(0, buf + buf_i, sizeof(char));
    if (count == 0 && buf_i == 0) break;
    if (buf[buf_i] == '\n' || count == 0) {
      buf[buf_i] = '\0';
      char * token = 0;
      for (int i = 0; i < buf_i; i++) {
        if (buf[i] == ' ' && (i !=0 && buf[i-1] != ' ')) {
          buf[i] = '\0';
          new_argv[arg_i++] = token;
          token = 0;
        }
        if (buf[i] != ' ' && (i == 0 || buf[i-1] == ' ' || buf[i-1] == '\0')) {
          token = buf+i;
        }
      }
      if (token != 0) {
        new_argv[arg_i++] = token;
      }
      new_argv[arg_i] = 0;
      // the new_argv is ready
      if (fork() == 0) {
        exec(new_argv[0], new_argv);
        exit(0);
      } else {
        wait(0);
        buf_i = 0;
        arg_i = argc - 1;
        continue;
      }
    }
    buf_i++;

    if (count == 0) {
      break;
    }
  }
  exit(0);
}
