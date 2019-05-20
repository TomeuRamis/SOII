#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        bread(0, &SB);
        char *buffer =malloc(sizeof(char));
        int tam = mi_dir(argv[2],buffer);
        write(1,buffer,tam);
        //fprintf(1,"%s",buffer);
        printf("\nTamaño: %d \n",tam);
        bumount();
    } else {
        printf("Sintaxi: ./mi_ls <disco> </ruta_directorio>\n");
    }
}