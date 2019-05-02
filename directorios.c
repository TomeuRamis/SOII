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
//p_inodo no se actualiza, es posible que al empezar p_inodo = p_inod_dir y se actualiza p_inodo_dir
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos){
    char *inicial;
    char *final;
    int nentrada;
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
    nentrada = 0;
    if(num_entradas>0){
        mi_read_f(p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada)); //Possible error por los size of entrada
             
        while( (nentrada < num_entradas)&&(inicial != entrada.nombre)){
            nentrada++;
            mi_read_f(p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada));
        }
    }
    if (nentrada==num_entradas) {
        switch (reservar) {
            case 0: return -3; //Error no exsiste entrada consulta
            case 1: //modo escritura
            if (strcmp(inodo.tipo, 'f')){
                return -4; //Error no se puede crear entrada en un fichero
            }
            if (inodo.permisos&&2==2){
                return -5; //Error permisos escritura
            } else {
                memcpy(entrada.nombre, inicial, sizeof(entrada.nombre));
                if (strcmp(inodo.tipo, 'd')){
                    if (final == '/'){
                        entrada.ninodo = reservar_inodo('d', permisos);
                    } else {
                        return -6; //Error no exsiste directorio intermedio
                    }
                } else {
                    entrada.ninodo = reservar_inodo('f', permisos);
                }
                int escritos = mi_write_f(p_inodo_dir, &entrada, num_entradas*sizeof(struct entrada), sizeof(struct entrada));
                if(escritos != sizeof(struct entrada)){
                    if (entrada.ninodo != -1){
                        liberar_inodo(entrada.ninodo);
                    }
                    return -7; //Error EXIT_FAILURE
                }
            }
            break;
        }

    }
    if( strcmp(final, "")){ //Puede que sea /0
        if ((nentrada < num_entradas)&&(reservar=1)) {
            return -8; //Error entrada ya exsistente
        }
        p_inodo = entrada.ninodo; //REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        p_entrada = nentrada;
        return 1; //Exit success
    } else {
        p_inodo_dir = entrada.ninodo;//REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        return buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos);
    }
}

int mi_creat(const char *camino, unsigned char permisos){
    unsigned int p_entrada =0;
    int error = buscar_entrada(camino,0,0,p_entrada,1,permisos);//Suponemos actualizacion de p_entrada
    switch (error){
        case -2:
            printf("Error con los permisos de lectura");
        break;
        case -5:
            printf("Error permisos de escritura");
        break;
        case -8:
            printf("Entrada ya existente");
        break;
    }
    return error;
}

int mi_dir(const char *camino, char *buffer){
    unsigned int *p_inodo = 0;
    unsigned int *p_entrada =0;
    struct inodo inodo;
    int nentradas;
    int offset = 0;
    int error = buscar_entrada(camino,0,p_inodo,p_entrada,1,4);
    if (error == -8) {
        leer_inodo(p_inodo,&inodo);
        if (inodo.tipo=='d' && inodo.permisos&&4==4){
            nentradas = (inodo.tamEnBytesLog/sizeof(struct entrada))+1;
        }
        for(int i = 0; i < nentradas, i++){
            offset+= mi_read_f(p_inodo,buffer,offset,sizeof(struct entrada));
            buffer+='|';
        }
    }
}