#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid  = getpid();
  int ppid = getppid();
  printf("[yosoytupadre] pid=%d, ppid=%d\n", pid, ppid);

  // (Opcional) mini prueba de paternidad con fork
  int c = fork();
  if (c == 0) {
    // Proceso hijo
    printf("[child]    pid=%d, ppid=%d (debe ser el pid del padre)\n", getpid(), getppid());
    exit(0);
  } else if (c > 0) {
    // Proceso padre
    printf("[parent]   pid=%d, child=%d\n", getpid(), c);
    wait(0);
    exit(0);
  } else {
    // Error en fork
    printf("fork failed\n");
    exit(1);
  }
}
