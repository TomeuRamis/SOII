#include "ficheros.h"
#include <math.h>

int main(int argc, char **argv){
    if (argc == 4){ //Si la sintaxi es correcta     
        bmount(argv[1]);
        bread(0, &SB);
        char *texto= argv[2];
        printf("Longitud texto: %ld\n\n", strlen(texto));
        unsigned char buffer[strlen(texto)];
        memcpy(buffer, texto, strlen(texto));
        int diferentes_inodos = strtol(argv[3], NULL, 0);
        int ninodo=reservar_inodo('f',6);
        const int offsets[5] = {0,5120, 256000, 30720000, 71680000};  
        for (int i = 0; i < 5; i++){ //Bucle para iterar los diferentes offsets           
            if (diferentes_inodos != 0 && i != 0){    //Si diferentes_inodos = 1 reservaremos un inodo en cada iteración
                ninodo= reservar_inodo('f',6);
            }
            printf("Nº inodo reservado: %d\n", ninodo);
            int offset = offsets[i];  
            printf("offset = %d\n",offset);
            int bytes_escritos = mi_write_f(ninodo,buffer,offset,strlen(texto)); //Escribimos el texto
            int bloques_escritos= (bytes_escritos/BLOCKSIZE);
            if (bytes_escritos % BLOCKSIZE){    //Calculamos si se necesita un bloque extra
                bloques_escritos++;
            }
            //Prints dels STAT del inodo despues de escribir el texto
            printf("Bytes escritos: %d\n\n",bytes_escritos);
            struct STAT *stat=malloc(sizeof(struct STAT));
            mi_stat_f(ninodo,stat); 
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
        }
        bumount();
    } else {
        printf("Sintaxis: escribir <nombre_dispositivo> <$(cat fichero)> <diferentes_inodos>");
        printf("Offsets: 0, 5120, 256000, 30720000, 71680000\n");
        printf("Si diferentes_inodos=0 se reserva un solo inodo para todos los offsets\n");
    }
}