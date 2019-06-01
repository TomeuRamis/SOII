#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        bread(0, &SB);
        char *camino = argv[2];
        if (strcmp(camino,"/")!=0){
            mi_unlink(argv[2]); 
        } else {
            printf("ERROR: No se puede borrar el directorio raiz\n");
        }
        bumount();
    } else {
        printf("Sintaxi: ./mi_rm disco </ruta_directorio>\n");
    }
}