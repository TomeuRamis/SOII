#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final){
    char *prinpal= strch(camino,'/');
    final = "";
    if (prinpal != '/'){
        printf("Los caminos deben empezar por /\n");
        return -1;
    }
    char *camino_aux= prinpal+1;
    int cont = 0;
    while (camino_aux!='/'||camino_aux!=NULL){
        camino_aux++;
        cont++;
    }
    strncpy(inicial,prinpal+1,cont);  
    if (camino_aux == NULL){
        return 0;
    } else {
        strcpy(final,camino_aux);
        return 1; 
    }
}

int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos){
    char *inicial;
    char *final;
    struct inodo inodo;
    struct entrada entrada;
    if (camino_parcial == '/'){
        p_inodo=0;
        p_entrada=0;
        return 0;
    }
    int tipo = extraer_camino(camino_parcial,inicial,final);
    if (tipo == -1){
        return -1; //Error al extraer camino (falta empezar por "/")
    }
    leer_inodo(p_inodo_dir,&inodo);
    if(permisos&&4!=4) {
        return -2; //Error de permisos de lectura
    }
    int num_entradas = inodo.tamEnBytesLog/sizeof(struct entrada);
    if(num_entradas>0){
        mi_read_f(entrada,) 
        while (){

        }
    }
    


}