#include "ficheros.h"

int TAMLEC = 1024; 

int main(int argc, char **argv){
    if (argc == 3){
        int leidos = 0;
        struct inodo inodo;
        bmount(argv[1]);
        bread(0, &SB);
        int ninodo = strtol(argv[2],NULL,0);
        char *buffer_texto= malloc(TAMLEC);
        //leer_inodo(ninodo,&inodo);
        int offset = 0;
        //printf("inodo.tamBytesLog = %d\n",inodo.tamEnBytesLog);
        while((leidos=mi_read_f(ninodo, buffer_texto, offset, TAMLEC))>0){
            write(1, buffer_texto, TAMLEC);
            offset = leidos;   //Esto esta mal espero acordarme de cambiarlo
        }
        /* while(offset<inodo.tamEnBytesLog){
            memset(buffer_texto,0,TAMLEC);
            leidos += mi_read_f(ninodo,buffer_texto,offset,TAMLEC);
            write(1,buffer_texto,TAMLEC);
            offset += TAMLEC;
        } */
        printf("bytes leidos = %d\n",leidos);
        bumount();
    } else {
        printf("Sintaxis: leer <nombre_dispositivo> <ninodo> \n");
    }  
    return 0;
}