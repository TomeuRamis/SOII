#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 5){

        //Comprobar que es un fichero
        char *camino= argv[2];
        char *camino_aux =argv[2];
        int cont =0;
        while (cont <strlen(camino)-1){
            camino_aux++;
            cont ++;
        }
        if(*camino_aux=='/'){
            printf("ERROR: La ruta no es un fichero\n");
            return 0;
        }else{
            bmount(argv[1]);
            bread(0, &SB);
            char *texto= argv[3];
            printf("Longitud texto: %ld\n", strlen(texto));
            unsigned char buffer[strlen(texto)];
            memcpy(buffer, texto, strlen(texto));  
            int offset = atoi(argv[4]);  
            unsigned int nbytes=strlen(texto);
            //printf("Longitud texto: %d",nbytes);
            int bytes_escritos = mi_write(camino,&buffer,offset,nbytes); //Escribimos el texto
            int bloques_escritos= (bytes_escritos/BLOCKSIZE);
            if (bytes_escritos % BLOCKSIZE){    //Calculamos si se necesita un bloque extra
                bloques_escritos++;
            }
            //Prints dels STAT del inodo despues de escribir el texto
            printf("Bytes escritos: %d\n",bytes_escritos);
            bumount();
        }
    } else {
        printf("Sintaxi: ./mi_write <disco> </ruta_fichero> <texto> <offset>\n");
    }
}