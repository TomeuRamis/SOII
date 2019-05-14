#include "ficheros.h"

int TAMLEC = 1500; 

int main(int argc, char **argv){
    int fd = 0;
    //Dependiendo de la sintaxi escribimos en un fichero o por pantalla
    if (argc == 5 || argc == 3){
        if (argc == 5){
            fd = open(argv[4], O_CREAT | O_RDWR);
        } else if(argc == 3){
            fd = 1;
        }
        int leidos = 0;
        int leido_aux = 0;
        unsigned int offset = 0;
        struct inodo inodo;
        int ninodo = strtol(argv[2],NULL,0);
        unsigned char buffer_texto[TAMLEC];

        bmount(argv[1]);
        bread(0, &SB);    
        memset(buffer_texto,0,TAMLEC);          //Inicializamos el buffer con 0s
        leido_aux = mi_read_f(ninodo, buffer_texto, offset, TAMLEC); //Guardamos los bytes leeidos en leido_aux
        leer_inodo(ninodo, &inodo);
        while(offset < inodo.tamEnBytesLog){ 
            write(fd, buffer_texto, leido_aux);     //Escribimos por pantalla o en el fichero lo que acabamos de leer
            leidos += leido_aux;                    //Actualizamos los bytes leidos totales
            offset += leido_aux;  
            leido_aux = mi_read_f(ninodo, buffer_texto, offset, TAMLEC);
        }
        write(fd, buffer_texto, leido_aux);   
        fprintf(stderr, "\nBytes leidos = %d\n",leidos);
        leer_inodo(ninodo,&inodo);
        fprintf(stderr, "TamEnBytesLog=%d\n",inodo.tamEnBytesLog);
        bumount();
    } else {
        printf("Sintaxis: leer <nombre_dispositivo> <ninodo>\n");
        printf("Se puede rdireccionar el contenido leido a un fichero: \n");
        printf("Sintaxis: leer <nombre_dispositivo> <ninodo> > <fichero>\n");
    }  
    return 0;
}