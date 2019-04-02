#include "ficheros.h"

int main(int argc, char **argv){
    if (argc ==4){
        bmount(argv[1]);
        bread(0,&SB);
        struct inodo inodo; 
        unsigned int ninodo = strtol(argv[2],NULL,0);
        unsigned int permisos = strtol(argv[3],NULL,0);
        leer_inodo(ninodo,&inodo);
        printf("Introducido permisos= %d",permisos);
        printf("Permisos = %d\n",inodo.permisos);
        mi_chmod_f(ninodo,permisos);
        printf("Permisos = %d\n",inodo.permisos);
        bumount();
    } else {
        printf("Sintaxis: truncar <nombre_dispositivo> <ninodo> <nbytes>");
    }
}