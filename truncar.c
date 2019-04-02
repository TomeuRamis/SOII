#include "ficheros.h"

int main(int argc, char **argv){
    if (argc ==4){
        bmount(argv[1]);
        bread(0,&SB);
        struct inodo inodo; 
        int ninodo = strtol(argv[2],NULL,0);
        int nbytes = strtol(argv[3],NULL,0);
        leer_inodo(ninodo,&inodo);
        printf("TamenByteslog = %d i BloquesOcupados = %d",inodo.tamEnBytesLog,inodo.numBloquesOcupados);
        mi_truncar_f(ninodo,nbytes);
        bumount();
        printf("TamenByteslog = %d i BloquesOcupados = %d",inodo.tamEnBytesLog,inodo.numBloquesOcupados);
        printf("Truncar.c ha finalizado\n");
    } else {
        printf("Sintaxis: truncar <nombre_dispositivo> <ninodo> <nbytes>");
    }
}