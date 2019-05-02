#include "directorios.c"

int main(int argc, char **argv){
    if (argc == 4){
        const char *ruta= argv[3]; 
        unsigned char permisos = argv[2];
        if (0<=permisos<=7) {
            mi_creat(ruta,permisos);
        }
    } else {
        printf("Sintaxis: ./mi_mkdir <disco> <permisos> </ruta>");
    }
}