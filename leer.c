#include "ficheros.h"

int TAMLEC = 1024; 

int main(int argc, char **argv){
    int fd = 0;
    if (argc == 5 || argc == 3){
        if (argc == 5){
            fd = open(argv[4], O_CREAT | O_RDWR);
        } else if(argc == 3){
            fd = 1;
        }
        int leidos = 0;
        struct inodo inodo;
        
        bmount(argv[1]);
        bread(0, &SB);
        int ninodo = strtol(argv[2],NULL,0);
        unsigned char buffer_texto[TAMLEC];
        leer_inodo(ninodo, &inodo);
        //printf("inodo.tamEnBytesLog = %d\n", inodo.tamEnBytesLog);
        int offset = 0;
        //printf("offset 1 : %d\n", offset);
        memset(buffer_texto,0,TAMLEC);
        //printf("inodo.tamBytesLog = %d\n",inodo.tamEnBytesLog);
        int leido_aux = mi_read_f(ninodo, buffer_texto, offset, TAMLEC);
        printf("leidos aux: %d\n", leido_aux);
        //printf("offset 2 : %d\n", offset);
        while(leido_aux>0){ 
        //while(offset<inodo.tamEnBytesLog){
            write(fd, buffer_texto, TAMLEC);
            fprintf(stderr, "\n");
            //printf("\n");
            //printf("offset 3 : %d\n", offset);
            memset(buffer_texto+leido_aux,0,TAMLEC-leido_aux);
            //printf("offset 4 : %d\n", offset);
            leidos += leido_aux;
            offset += leido_aux;   //Esto esta mal espero acordarme de cambiarlo
            //printf("offset en leer.c : %d\n", offset);
            leido_aux = mi_read_f(ninodo, buffer_texto, offset, TAMLEC);
            //printf("offset 6 : %d\n", offset);
        }
        fprintf(stderr, "bytes leidos = %d\n",offset);
        leer_inodo(ninodo,&inodo);
        fprintf(stderr, "tamEnBytesLog=%d\n",inodo.tamEnBytesLog);
        bumount();
    } else {
        printf("Sintaxis: leer <nombre_dispositivo> <ninodo>\n");
        printf("Se puede rdireccionar el contenido leido a un fichero: \n");
        printf("Sintaxis: leer <nombre_dispositivo> <ninodo> > <fichero>\n");
    }  
    return 0;
}