Makefile

Este proyecto incluye un Makefile para facilitar la compilación, ejecución y comprobación del programa.

Comandos disponibles
Compilar el proyecto
make


Compila todos los archivos necesarios y genera el ejecutable shell.

También se puede utilizar:

make all


Ambos comandos tienen el mismo efecto.

Ejecutar el programa
make run


Compila el proyecto si es necesario y ejecuta el programa shell.

Ejecutar con Valgrind
make valgrind


Compila el proyecto si es necesario y ejecuta shell utilizando Valgrind para detectar posibles errores relacionados con la memoria.

Este comando utiliza:

valgrind --show-reachable=yes --leak-check=full ./shell

Limpiar el proyecto
make clean


Elimina todos los archivos .o generados durante la compilación y el ejecutable shell.

Resumen
Comando	Función
make	Compila el proyecto.
make all	Compila el proyecto.
make run	Compila y ejecuta shell.
make valgrind	Compila y ejecuta shell con Valgrind.
make clean	Elimina los archivos generados.
Ejemplo de uso

Para empezar desde cero:

make clean
make
make run


Para comprobar posibles errores de memoria:

make valgrind
