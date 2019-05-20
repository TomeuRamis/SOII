#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 4){
        bmount(argv[1]);
            bread(0, &SB);
        
        char *camino1 = argv[2];
        char *camino2 = argv[3];

        //Comprobar que es un fichero
        char *camino_aux1 = camino1;
        char *camino_aux2 = camino2;
        int cont =0;
        while (cont <strlen(camino1)-1){
            camino_aux1++;
            cont ++;
        }
        cont =0;
        while (cont <strlen(camino2)-1){
            camino_aux2++;
            cont ++;
        }
        if((*camino_aux1=='/')||(*camino_aux2=='/')){
            printf("La ruta no es un fichero");
            return 0;
        } else {
            
            
            mi_link(camino1,camino2);

        }
        bumount();
    } else {
        printf("Sintaxis: ./mi_link disco </ruta_fichero_original> </ruta_enlace>\n");
    }
}