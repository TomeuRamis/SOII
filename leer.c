#include "ficheros.h"

int TAMLEC = 1024; 

int main(int argc, char **argv){
    int leidos = 0;
    struct inodo inodo;
    bmount(argv[1]);
    bread(0, &SB);
    int ninodo = strtol(argv[2],NULL,0);
    printf("ninodo %d\n",ninodo);
    char *buffer_texto= malloc(TAMLEC);
    leer_inodo(ninodo,&inodo);
    int offset = 0;
    printf("inodo.tamBytesLog = %d\n",inodo.tamEnBytesLog);
    while(offset<inodo.tamEnBytesLog){
        memset(buffer_texto,0,TAMLEC);
        leidos += mi_read_f(ninodo,buffer_texto,offset,TAMLEC);
        write(1,buffer_texto,TAMLEC);
        offset += TAMLEC;
    }
    bumount();
    printf("bytes leidos = %d\n",leidos);
    return 0;
}