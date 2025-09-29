#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid = getpid();
  int ppid = getppid();
  printf("Mi PID: %d, PPID: %d\n", pid, ppid);
  exit(0);
}