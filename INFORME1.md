# Tarea 1 – Sistemas Operativos (xv6-riscv)

**Autor:** mikelVicente  
**Rama de entrega:** `users/mikelVicente/syscall-getppid`  

---

## Funcionamiento de las llamadas al sistema

### `getppid()`
Esta llamada retorna el **PID del proceso padre** del proceso actual.  
- Si el proceso tiene padre (lo normal en xv6), retorna ese PID.  
- Si el proceso no tiene padre (caso del proceso `init`), retorna 0.  

Ejemplo de ejecución en xv6 con el programa `yosoytupadre`:

[yosoytupadre] pid=3, ppid=2
[child] pid=4, ppid=3
[parent] pid=3, child=4

Se observa que el proceso hijo (`pid=4`) muestra como `ppid=3`, que corresponde al padre.

### `getancestor(int n)`
Esta llamada retorna el **PID del ancestro en nivel `n`**:
- `n = 0` → el mismo proceso.  
- `n = 1` → el padre.  
- `n = 2` → el abuelo, etc.  
- Si `n` es negativo o no existe tal ancestro, retorna `-1`.  

Ejemplo de ejecución en xv6 con el programa `ancestros`:

getancestor(0) = 3
getancestor(1) = 2
getancestor(2) = 1
getancestor(3) = -1
getancestor(4) = -1
[child] pid=4, ppid=3, ancestor(1)=3
[parent] pid=3, ancestor(0)=3

La salida confirma que cada nivel retorna el PID correcto y que el hijo reconoce al padre como ancestro.

---

## Explicación de las modificaciones realizadas

1. **`kernel/syscall.h`**  
   - Se añadieron los números de syscall:
     ```c
     #define SYS_getppid     22
     #define SYS_getancestor 23
     ```

2. **`kernel/syscall.c`**  
   - Se agregaron los `extern`:
     ```c
     extern uint64 sys_getppid(void);
     extern uint64 sys_getancestor(void);
     ```
   - Se incluyeron en la tabla de despacho:
     ```c
     [SYS_getppid]     sys_getppid,
     [SYS_getancestor] sys_getancestor,
     ```

3. **`kernel/sysproc.c`**  
   - Implementación de `sys_getppid()`:
     ```c
     uint64 sys_getppid(void) {
       struct proc *p = myproc();
       return p->parent ? p->parent->pid : 0;
     }
     ```
   - Implementación de `sys_getancestor()`:
     ```c
     uint64 sys_getancestor(void) {
       int n;
       argint(0, &n);
       if (n < 0) return -1;
       struct proc *p = myproc();
       while (n > 0 && p) {
         p = p->parent;
         n--;
       }
       return p ? p->pid : -1;
     }
     ```

4. **`user/user.h`**  
   - Se añadieron los prototipos:
     ```c
     int getppid(void);
     int getancestor(int);
     ```

5. **`user/usys.pl`**  
   - Se añadieron las entradas:
     ```perl
     entry("getppid");
     entry("getancestor");
     ```

6. **Programas de usuario**
   - `user/yosoytupadre.c`: prueba de `getppid()`.  
   - `user/ancestros.c`: prueba de `getancestor(int)`.  
   - Se actualizaron las entradas en el `Makefile` para compilar estos programas.

---

## Dificultades encontradas y cómo se resolvieron

- **Error de compilación “return type defaults to int” en `sys_getppid`:**  
  Se debió a que la función estaba sin tipo de retorno explícito. Se corrigió declarando `uint64` como tipo de retorno.

- **“exec ... failed” al ejecutar `yosoytupadre`:**  
  El archivo no estaba guardado o no se había agregado correctamente al `Makefile`. Se solucionó asegurando que `user/yosoytupadre.c` estuviera guardado y que `$U/_yosoytupadre\` se agregara a `UPROGS`.

- **Error “void value not ignored” en `sys_getancestor`:**  
  En esta versión de xv6 la función `argint` retorna `void`. Se corrigió eliminando la comparación `if (argint(...) < 0)` y validando directamente el valor leído.

- **Salidas intercaladas entre padre e hijo en los `printf`:**  
  Es un comportamiento esperado por concurrencia. Para obtener salidas más ordenadas en el informe, se utilizó `wait(0)` para que el padre imprima después de que termine el hijo.

---

## Conclusión
Las llamadas al sistema `getppid()` y `getancestor(int)` fueron implementadas exitosamente en xv6, y los programas de prueba verifican su funcionamiento correcto.  
Se documentaron los cambios realizados y las dificultades enfrentadas, logrando una implementación robusta y explicada para la entrega de la tarea.
