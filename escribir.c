#include "ficheros.h"
#include <math.h>

int main(int argc, char **argv){
    if (argc == 4){
        struct inodo inodo;        
        bmount(argv[1]);
        bread(0, &SB);
        //printf("Posición cantInodosLibres: %d\n", SB.cantInodosLibres);
        int ninodo= reservar_inodo('f',6);
        leer_inodo(ninodo,&inodo);
        printf("Numero inodo: %d\n", ninodo);
        int offset = 0;
        char *texto= argv[2];
        char buffer[strlen(texto)];
        printf("Strlen()texto = %ld\n", strlen(texto));
        int bloques_escritos_aux = mi_write_f(ninodo,texto,offset,strlen(texto));
        int bloques_escritos= (bloques_escritos_aux/BLOCKSIZE);
        if (bloques_escritos_aux % BLOCKSIZE){
            bloques_escritos++;
        }
        printf("\nBloques escritos: %d\n",bloques_escritos);
        struct STAT *stat =malloc(sizeof(struct STAT));
        mi_stat_f(ninodo,stat); 
         
        //printf("Num bloques ocupados: %d\n", inodo.numBloquesOcupados);
        fprintf(stderr,"Tamaño en bytes logicos : %d\n",stat ->tamEnBytesLog);
        //fprintf(stderr,"Número de bloques ocupados: %d\n", stat ->numBloquesOcupados);
        printf("\n\nPrints mi_read_f\n");
        memset(buffer,0,strlen(texto));
        mi_read_f(ninodo,buffer,offset,strlen(texto));
        write(1,buffer,strlen(texto));
        //printf("contenido escrito: %s\n", &buffer );
        //int fd = open("texto.txt", O_RDWR | O_CREAT, S_IRUSR | S_IROTH);
        //write(fd,buffer,bloques_escritos*BLOCKSIZE);
        // escribir_inodo(ninodo,inodo);
        printf("Contenido punteros directos : %d\n",inodo.punterosDirectos[0]);
        /* printf("\n\n----------------------------------------------------------\n");
        int TAMLEC=BLOCKSIZE;
        int leidos=0;
        char *buffer_texto= malloc(TAMLEC);
        //leer_inodo(ninodo,&inodo);
        offset = 0;
        int i =1;
        printf("inodo.tamBytesLog = %d\n",inodo.tamEnBytesLog);
        while(offset<inodo.tamEnBytesLog){
            printf("Iteración %d\n",i);
            i++;
            memset(buffer_texto,0,TAMLEC);
            leidos += mi_read_f(ninodo,buffer_texto,offset,TAMLEC);
            write(1,buffer_texto,TAMLEC);
            offset += TAMLEC;
        }
        printf("bytes leidos = %d\n",leidos); */
        bumount();
    } else {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>");
    }
}