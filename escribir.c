#include "ficheros.h"

int main(int argc, char **argv){
    if (argc == 4){
        struct inodo inodo;
        bmount(argv[1]);
        bread(0, &SB);
        //printf("Posición cantInodosLibres: %d\n", SB.cantInodosLibres);
        int ninodo= reservar_inodo('f',6);
        printf("Numero inodo: %d\n", ninodo);
        int offset = 0;
        char *texto= argv[2];
        char buffer[strlen(texto)];
        printf("Strlen()texto = %ld\n", strlen(texto));
        int bloques_escritos = mi_write_f(ninodo,texto,offset,strlen(texto))/BLOCKSIZE;
        printf("Bloques escritos: %d\n",bloques_escritos);
        struct STAT *stat =malloc(sizeof(struct STAT));
        mi_stat_f(ninodo,stat); 
        leer_inodo(ninodo,&inodo); 
        //printf("Num bloques ocupados: %d\n", inodo.numBloquesOcupados);
        fprintf(stderr,"Tamaño en bytes logicos : %d\n",stat ->tamEnBytesLog);
        //fprintf(stderr,"Número de bloques ocupados: %d\n", stat ->numBloquesOcupados);
        mi_read_f(ninodo,buffer,offset,strlen(texto));
        //int fd = open("texto.txt", O_RDWR | O_CREAT, S_IRUSR | S_IROTH);
        //write(fd,buffer,bloques_escritos*BLOCKSIZE);
        bumount();
    } else {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>");
    }
}