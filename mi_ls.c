#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        bread(0, &SB);
        char *buffer = malloc(1000000);
        mi_dir(argv[2],buffer);
        write(1,buffer,sizeof(buffer));
        bumount();
    } else {
        printf("Sintaxi: ./mi_ls <disco> </ruta_directorio>");
    }
}