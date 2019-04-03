#include "ficheros.h"
#include <math.h>

int main(int argc, char **argv){
    if (argc == 4){
        //struct inodo inodo;        
        bmount(argv[1]);
        bread(0, &SB);
        char *texto= argv[2];
        printf("longitud texto: %ld\n\n", strlen(texto));
        unsigned char buffer[strlen(texto)];
        //printf("Strlen()texto = %ld\n", strlen(texto));
        int reservar_inodos = strtol(argv[3], NULL, 0);
        int ninodo=reservar_inodo('f',6);
        //printf("Posición cantInodosLibres: %d\n", SB.cantInodosLibres);
        const int offsets[5] = {0,5120, 256000, 30720000, 71680000};  
        for (int i = 0; i < 5; i++){
            
            if (reservar_inodos != 0 && i != 0){
                ninodo= reservar_inodo('f',6);
            }
            printf("Numero inodo reservado: %d\n", ninodo);
            //leer_inodo(ninodo,&inodo);
            //printf("Numero inodo: %d\n", ninodo);
            int offset = offsets[i];  
            printf("offset = %d\n",offset);
            int bytes_escritos = mi_write_f(ninodo,texto,offset,strlen(texto));
            int bloques_escritos= (bytes_escritos/BLOCKSIZE);
            if (bytes_escritos % BLOCKSIZE){
                bloques_escritos++;
            }
            printf("Bytes escritos: %d\n\n",bytes_escritos);
            struct STAT *stat=malloc(sizeof(struct STAT));
            mi_stat_f(ninodo,stat); 
           // printf("Bloques escritos: %d\n",bloques_escritos);
            printf("DATOS INODO %d:\n", ninodo);
            printf("tipo=%c\n",stat ->tipo);
            printf("permisos=%d\n",stat ->permisos);
            struct tm *ts;
            char atime[80];
            char mtime[80];
            char ctime[80];
            ts = localtime(&stat ->atime);         
            strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
            ts = localtime(&stat ->mtime);
            strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
            ts = localtime(&stat ->ctime);
            strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
            printf("ATIME: %s\nMTIME: %s\nCTIME: %s\n",atime,mtime,ctime); 
            printf("nlinks=%d\n",stat ->nlinks);
            printf("tamEnBytesLong=%d\n", stat ->tamEnBytesLog);
            printf("numBloquesOcupados=%d\n\n",stat ->numBloquesOcupados);

            //printf("Num bloques ocupados: %d\n", inodo.numBloquesOcupados);
            //fprintf(stderr,"Tamaño en bytes logicos : %d\n",stat ->tamEnBytesLog);
            //fprintf(stderr,"Número de bloques ocupados: %d\n", stat ->numBloquesOcupados);
            //printf("\n\nPrints mi_read_f\n");
            //memset(buffer,0,strlen(texto));
            //mi_read_f(ninodo,buffer,offset,strlen(texto));
            //write(1,buffer,strlen(texto));
            //printf("\n");
            //printf("contenido escrito: %s\n", &buffer );
            //int fd = open("texto.txt", O_RDWR | O_CREAT, S_IRUSR | S_IROTH);
            //write(fd,buffer,bloques_escritos*BLOCKSIZE);
            //escribir_inodo(ninodo,inodo);
            //printf("Contenido punteros directos : %d\n",inodo.punterosDirectos[0]);
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
        }
        bumount();
    } else {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>");
        printf("Offsets: 0, 5120, 256000, 30720000, 71680000\n");
        printf("Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
    }
}