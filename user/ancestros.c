#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  // Imprime una pequeña tabla de ancestros del proceso actual
  for (int k = 0; k <= 4; k++) {
    int a = getancestor(k);
    printf("getancestor(%d) = %d\n", k, a);
  }

  // Demuestra con fork la relación padre/hijo
  int c = fork();
  if (c == 0) {
    // hijo
    printf("[child]  pid=%d, ppid=%d, ancestor(1)=%d\n",
           getpid(), getppid(), getancestor(1));
    exit(0);
  } else if (c > 0) {
    // padre
    wait(0);
    printf("[parent] pid=%d, ancestor(0)=%d\n", getpid(), getancestor(0));
    exit(0);
  } else {
    printf("fork failed\n");
    exit(1);
  }
}