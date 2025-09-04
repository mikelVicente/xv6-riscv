# INFORME – Tarea 0: Instalación y ejecución de xv6

## Pasos seguidos para instalar xv6

### 1) Preparación del entorno
- **Sistema:** Windows 11 + **WSL2**.
- **Distribuciones WSL disponibles:** `Ubuntu` y `Ubuntu-24.04`.
- **Carpeta de trabajo:** `~/proyectos` **dentro de Ubuntu (WSL)** para evitar problemas de OneDrive.
- Verifiqué versiones:
  ```bash
  qemu-system-riscv64 --version     # debe ser >= 7.2
  riscv64-linux-gnu-gcc --version
### 2) Clonado del repositorio oficial y rama de trabajo
mkdir -p ~/proyectos
cd ~/proyectos
git clone https://github.com/mit-pdos/xv6-riscv.git
cd xv6-riscv
git switch -c mikel_t0     # rama propia para la tarea
### 3) Instalación de dependencias en Ubuntu (WSL)
sudo apt update
sudo apt install -y build-essential gdb-multiarch qemu-system-misc git \
                    gcc-riscv64-linux-gnu binutils-riscv64-linux-gnu
build-essential: herramientas de compilación (gcc, make, etc.).

gdb-multiarch: depurador multi-arquitectura (incluye RISC-V).

qemu-system-misc: trae qemu-system-riscv64.

gcc/binutils riscv64-linux-gnu: toolchain cruzado para RISC-V.

### 4) Compilación de xv6 (RISC-V)
make clean
make TOOLPREFIX=riscv64-linux-gnu-
TOOLPREFIX=riscv64-linux-gnu- obliga a usar el compilador/ensamblador RISC-V (no x86).

### 5) Ejecución en QEMU

make qemu TOOLPREFIX=riscv64-linux-gnu-
Si se abre en ventana: cerrar la ventana para salir.

Si se ejecuta en modo texto (-nographic): salir con Ctrl + a y luego x.
Si el terminal intercepta Ctrl+a (pasa en Windows Terminal), desde otra ventana se puede cerrar con:

pkill -f qemu-system-riscv64
(o desde PowerShell: wsl -d Ubuntu -- pkill -f qemu-system-riscv64).

## Problemas encontrados y soluciones
“Couldn't find a riscv64 version of GCC/binutils” / errores tipo “no such instruction (csrr, la, …)”
Causa: se estaba usando el toolchain de x86 por defecto.
Solución: instalé gcc-riscv64-linux-gnu y binutils-riscv64-linux-gnu y compilé con TOOLPREFIX=riscv64-linux-gnu-.

“ERROR: Need qemu version ≥ 7.2”
Causa: QEMU 6.x en Ubuntu 22.04 era demasiado antiguo.
Solución: usar un QEMU más nuevo (por ejemplo el de Ubuntu-24.04, que es 8.x). Verifiqué con:

qemu-system-riscv64 --version
which qemu-system-riscv64
y aseguré que el ejecutable que se usa al lanzar make qemu sea el nuevo.

Carpeta en OneDrive / rutas de Windows
Riesgo: sincronización/permisos rompen make y/o QEMU.
Solución: mover el proyecto a ~/proyectos dentro de WSL y trabajar ahí.

Salir de QEMU cuando Ctrl+a no funciona
Causa: el terminal capturaba Ctrl+a.
Solución: cerrar QEMU desde otra terminal con pkill -f qemu o ejecutar en modo ventana.

## Confirmación de que xv6 está funcionando correctamente
Al ejecutar make qemu TOOLPREFIX=riscv64-linux-gnu- se observó el arranque:

csharp
xv6 kernel is booting
hart 2 starting
hart 1 starting
init: starting sh
$
Con el prompt de xv6 ($) corrí exactamente:

ls
echo "Hola xv6"
cat README
Resultados esperados/observados:

ls lista programas de usuario (_cat, _echo, _sh, etc.) y archivos del FS.

echo "Hola xv6" imprime la cadena en la consola de xv6.

cat README muestra el archivo README incluido en la imagen de usuario.

Con esto se verifica E/S de consola, listado y lectura de archivos, por lo que xv6 queda correctamente instalado y funcionando en mi entorno (WSL2 + QEMU RISC-V) y el trabajo se registró en la rama mikel_t0.

