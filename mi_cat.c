#include "directorios.h"

int tambuffer = 1500;

int main(int argc, char **argv){
    if (argc == 2){
        
        //Comprobar que es un fichero
        char *camino= argv[2];
        char *camino_aux =argv[2];
        int cont =0;
        while (cont <strlen(camino)-1){
            camino_aux++;
            cont ++;
        }
        if(*camino_aux=='/'){
            printf("La ruta no es un fichero");
            return 0;
        } else {

            int leidos = 0;
            int leido_aux = 0;
            unsigned int offset = 0;
            struct inodo inodo;
            int ninodo = strtol(argv[2],NULL,0);
            unsigned char buffer_texto[tambuffer];
            char *camino=argv[2];

            bmount(argv[1]);
            bread(0, &SB);  
            memset(buffer_texto,0,tambuffer);          //Inicializamos el buffer con 0s
            leido_aux = mi_read(camino, buffer_texto, offset, tambuffer); //Guardamos los bytes leeidos en leido_aux
            leer_inodo(ninodo, &inodo);
            while(offset < inodo.tamEnBytesLog){ 
                write(camino, buffer_texto, leido_aux);     //Escribimos por pantalla o en el fichero lo que acabamos de leer
                leidos += leido_aux;                    //Actualizamos los bytes leidos totales
                offset += leido_aux;  
                leido_aux = mi_read(camino, buffer_texto, offset, tambuffer);
            }
            write(camino, buffer_texto, leido_aux);   
            fprintf(stderr, "\nBytes leidos = %d\n",leidos);
            leer_inodo(ninodo,&inodo);
            fprintf(stderr, "TamEnBytesLog=%d\n",inodo.tamEnBytesLog);
            bumount();
        }

    } else {
        printf("Sintaxi: ./mi_cat <disco> </ruta_fichero>\n");
    }
}