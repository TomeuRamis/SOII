#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 4){
        bmount(argv[1]);
        const char *ruta = argv[3]; 
        //printf("%s\n", ruta);
        unsigned char permisos = atoi(argv[2]);
        if ((0<=permisos)&&(permisos<=7)) {
            //printf("Llego a: mi_mkdir permisos correctos\n");
            mi_creat(ruta,permisos);
        }
        bumount();
    } else {
        printf("Sintaxis: ./mi_mkdir <disco> <permisos> </ruta>\n");
    }
}