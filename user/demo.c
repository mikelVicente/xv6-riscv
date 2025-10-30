#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int n = 10;
  for (int i = 0; i < n; i++) {
    int pid = fork();
    if (pid == 0) {
      settickets(50 * (i + 1));   // 50, 100, 150, ...
      // Busy work para consumir CPU
      volatile uint64 x = 0;
      for (volatile uint64 j = 0; j < 200000000ULL; j++) x += j;
      printf("child pid=%d tickets=%d done\n", getpid(), 50*(i+1));
      exit(0);
    }
  }
  for (int i = 0; i < n; i++) wait(0);
  exit(0);
}
