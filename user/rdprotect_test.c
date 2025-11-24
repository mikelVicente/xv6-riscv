#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  char *addr;

  // Obtener el puntero actual del heap
  addr = sbrk(0);
  if (addr == SBRK_ERROR) {
    printf("sbrk(0) error\n");
    exit(1);
  }

  // Reservar una página más (4096 bytes)
  if (sbrk(4096) == SBRK_ERROR) {
    printf("sbrk(4096) error\n");
    exit(1);
  }

  addr[0] = 'Z';  // escribir algo inicial

  // Proteger contra lectura 1 página
  if (mrdprotect(addr, 1) < 0) {
    printf("mrdprotect falló\n");
    exit(1);
  }

  // Escritura aún debería estar permitida
  addr[0] = 'A';

  // Esta lectura debería provocar fallo de página / matar el proceso
  char c = addr[0];
  printf("Valor leído: %c (esto NO debería imprimirse)\n", c);

  // Si por alguna razón llegáramos acá, revertimos protección
  if (munrdprotect(addr, 1) < 0) {
    printf("munrdprotect falló\n");
    exit(1);
  }

  printf("Protección revertida correctamente.\n");
  exit(0);
}
