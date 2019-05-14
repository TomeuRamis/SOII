#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 4){
        bmount(argv[1]);
        bread(0, &SB);
        unsigned char permisos = atoi(argv[2]);
        if ((0<=permisos)&&(permisos<=7)){
            mi_chmod(argv[3],permisos);
        }
        bumount();
    } else {
        printf("Sintaxi: ./mi_chmod <disco> <permisos> </ruta>");
    }
}