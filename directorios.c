#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final){
    printf("Llego a: extraer_camino\n");
    char *prinpal= strchr(camino,'/');
    final = "";
    inicial="";
    if (!strcmp(prinpal,"/")){
        printf("Los caminos deben empezar por /\n");
        return -1;
    }
    printf("Llego a: ext_cam el directorio es correcto\n");
    char *camino_aux= prinpal+1;
    int cont = 0;
    while ((strcmp(camino_aux,"/")!=0)&&(camino_aux[cont]!='\0')){
        camino_aux++;
        cont++;
    }
    printf("Llego a: extraer camino busco entrada (%d)\n",cont);
    strncpy(inicial,prinpal+1,cont);  
          printf("%s\n", inicial);
    if (camino_aux[cont]=='\0'){
        printf("Llego a: ext_cam no se encuentra directorio\n");
        printf("Inicial: %s, Final: %s",inicial,final);
        return 0;
    } else {
        printf("Llego a: ext_cam se encontro directorio\n");
        strcpy(final,camino_aux);
        //printf("Inicial: %s, Final: %s",inicial,final);
        return 1; 
    }
}
//p_inodo no se actualiza, es posible que al empezar p_inodo = p_inod_dir y se actualiza p_inodo_dir
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos){
    printf("Llego a: buscar_entrada \n");
    char *inicial = NULL;
    char *final = (char*)camino_parcial;//caution
    int nentrada;
    struct inodo inodo;
    struct entrada entrada;
    if (strcmp(camino_parcial,"/")==0){
        printf("Llego a: buscar entrada directorio raiz\n");
        p_inodo=0;
        p_entrada=0;
        return 0;
    }
    int tipo = extraer_camino(camino_parcial,inicial,final);
    if (tipo == -1){
        return -1; //Error al extraer camino (falta empezar por "/")
    }
    printf("buscar_entrada()→ inicial: %s, final: %s, reservar: %d",inicial,final,reservar);
    leer_inodo(*p_inodo_dir,&inodo);
    if(permisos&&4!=4) {
        printf("Error de permisos de lectura");
        return -2; //Error de permisos de lectura
    }
    int num_entradas = inodo.tamEnBytesLog/sizeof(struct entrada);
    nentrada = 0;
    if(num_entradas>0){
        mi_read_f(*p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada)); //Possible error por los size of entrada
             
        while( (nentrada < num_entradas)&&(inicial != entrada.nombre)){
            nentrada++;
            mi_read_f(*p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada));
        }
    }
    if (nentrada==num_entradas) {
        switch (reservar) {
            case 0:
                printf("Error no exsiste entrada consulta"); 
                return -3; //Error no exsiste entrada consulta
            case 1: //modo escritura
            if (strcmp(&inodo.tipo, "f")){
                printf("Error no se puede crear entrada en un fichero");
                return -4; //Error no se puede crear entrada en un fichero
            }
            if (inodo.permisos&&2==2){
                printf("Error permisos escritura");
                return -5; //Error permisos escritura
            } else {
                memcpy(entrada.nombre, inicial, sizeof(entrada.nombre));
                if (strcmp(&inodo.tipo, "d")){
                    if (strcmp(final,"/")){
                        entrada.ninodo = reservar_inodo('d', permisos);
                    } else {
                        printf("Error no exsiste directorio intermedio");
                        return -6; //Error no exsiste directorio intermedio
                    }
                } else {
                    entrada.ninodo = reservar_inodo('f', permisos);
                }
                int escritos = mi_write_f(*p_inodo_dir, &entrada, num_entradas*sizeof(struct entrada), sizeof(struct entrada));
                if(escritos != sizeof(struct entrada)){
                    if (entrada.ninodo != -1){
                        liberar_inodo(entrada.ninodo);
                    }
                    printf("Error EXIT_FAILURE");
                    return -7; //Error EXIT_FAILURE
                }
            }
            break;
        }

    }
    if( strcmp(final, "")){ //Puede que sea /0
        if ((nentrada < num_entradas)&&(reservar=1)) {
            printf("Error entrada ya exsistente");
            return -8; //Error entrada ya exsistente
        }
        p_inodo = &entrada.ninodo; //REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        *p_entrada = nentrada;//Quiza es sospechoso el *, añadido como parche
        return 1; //Exit success
    } else {
        p_inodo_dir = &entrada.ninodo;//REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        return buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos);
    }
}

int mi_creat(const char *camino, unsigned char permisos){
    printf("Llego a:mi creat\n");
    unsigned int *p_entrada =0;
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
        case 0:
            printf("Llego a: mi_crt camino es directorio raiz\n");
        break;
        case 1:
            printf("Llego a: mi_creat: exit success\n");
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
    int error = buscar_entrada(camino,0,p_inodo,p_entrada,0,4);
    if (error == 1) {
        leer_inodo(*p_inodo,&inodo);
        if (inodo.tipo=='d' && inodo.permisos&&4==4){
            nentradas = (inodo.tamEnBytesLog/sizeof(struct entrada))+1;
        }
        for(int i = 0; i < nentradas; i++){
            offset+= mi_read_f(*p_inodo,buffer,offset,sizeof(struct entrada));
            buffer+='|';
        }
        return 0;
    }
    return -1;
}

int mi_chmod(const char *camino, unsigned char permisos){
    unsigned int *p_inodo = 0;
    unsigned int *p_entrada =0;
    int error = buscar_entrada(camino,0,p_inodo,p_entrada,0,4);
    if (error ==1) {
        mi_chmod_f(*p_inodo,permisos);
        return 0;
    }
    return -1;
}

int mi_stat(const char *camino, struct STAT *p_stat){
     unsigned int *p_inodo = 0;
    unsigned int *p_entrada =0;
    int error = buscar_entrada(camino,0,p_inodo,p_entrada,0,4);
    if (error ==1) {
        mi_stat_f(*p_inodo,p_stat);
        return 0;
    }
    return -1;
}