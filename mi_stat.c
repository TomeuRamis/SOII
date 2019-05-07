#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 2){
        bmount(argv[1]);
        struct STAT *stat = malloc(sizeof(struct STAT));
        mi_stat(argv[2],stat);
        bumount();
    } else {
        printf("Sintaxi: ./mi_stat <disco> </ruta>");
    }
}