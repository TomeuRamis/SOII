#include "directorios.h"

int extraer_camino(const char *camino, char *inicial, char *final){
    //printf("Llego a: extraer_camino\n");
    char *prinpal= malloc(sizeof(strlen(camino)));
    prinpal = strchr(camino,'/');
    //inicial="";
    if (!strcmp(prinpal,"/")){
        printf("Los caminos deben empezar por /\n");
        return -1;
    }
    //printf("Llego a: ext_cam el directorio es correcto\n");
    char *camino_aux= malloc(sizeof(strlen(camino))); 
    prinpal++;
    camino_aux = prinpal;
    //printf("Prinpal: %s",*prinpal);
    
    camino_aux = strchr(camino_aux, '/');
    int cont = camino_aux-prinpal;
    //camino_aux--;

    //printf("Llego a: extraer camino busco entrada (%d)\n",cont);
    //inicial = prinpal+1;                                    // EXtremadamente cutre
    strncpy(inicial,prinpal,cont);
    //inicial = strndup(inicial, cont-1);  
    //printf("INicial :%s\n", inicial);
    //printf("Camino_aux: %c\n",*camino_aux);
    if (*camino_aux=='\0'){ // Quiza cont-1
        //printf("Llego a: ext_cam el camino termina en fichero\n");
        printf("Inicial: %s, Final: %s",inicial,final);
        return 0;
    } else {
        //printf("Llego a: ext_cam es un directorio %s %s\n",final,camino_aux);
        strcpy(final,camino_aux);
        printf("Inicial: %s, Final: %s\n",inicial,final);
        return 1; 
    }
}
//p_inodo no se actualiza, es posible que al empezar p_inodo = p_inod_dir y se actualiza p_inodo_dir
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos){
    printf("Llego a: buscar_entrada \n");
    struct entrada entrada;

    // char inical[sizeof(entrada.nombre)];
    // memset(inical, 0, sizeof(entrada.nombre));
    // char final[strlen(camino_parcial)];
    // memset(final, 0, strlen(camino_parcial));
    char *inicial= malloc(sizeof(char));
    char *final=malloc(sizeof(char));

    int nentrada;    
    struct inodo inodo;
    if (strcmp(camino_parcial,"/")==0){
        printf("Llego a: buscar entrada directorio raiz\n");
        *p_inodo=0;
        *p_entrada=0;
        return 0;
    }
    int tipo = extraer_camino(camino_parcial,inicial,final);
    if (tipo == -1){
        return -1; //Error al extraer camino (falta empezar por "/")
    }
    printf("[buscar_entrada()→ inicial: %s, final: %s, reservar: %d]\n",inicial,final,reservar);
    leer_inodo(*p_inodo_dir,&inodo);//no lee el inodo que toca
    if(permisos&&4!=4) {
        printf("Error de permisos de lectura");
        return -2; //Error de permisos de lectura
    }
    int num_entradas = inodo.tamEnBytesLog/sizeof(struct entrada);
    nentrada = 0;
    if(num_entradas>0){
        mi_read_f(*p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada)); //Possible error por los size of entrada
            nentrada++;
        while( (nentrada < num_entradas)&&(strcmp(inicial, entrada.nombre) != 0)) {
            //printf("El de abajo miente\n");
            mi_read_f(*p_inodo_dir, &entrada, nentrada*sizeof(struct  entrada), sizeof(struct entrada));
            nentrada++;
        }
    }
    leer_inodo(entrada.ninodo, &inodo);
    char itipo= inodo.tipo;
    if (nentrada==num_entradas && strcmp(inicial, entrada.nombre) != 0) {
        switch (reservar) {
            case 0:
                printf("Error no exsiste entrada consulta");
                return -3; //Error no exsiste entrada consulta
            case 1: //modo escritura
            if (itipo=='f'){
                printf("Error no se puede crear entrada en un fichero");
                return -4; //Error no se puede crear entrada en un fichero
            }
            if (inodo.permisos&&2!=2){
                printf("Error permisos escritura");
                return -5; //Error permisos escritura
            } else {
                memcpy(entrada.nombre, inicial, sizeof(entrada.nombre));
                if (itipo=='d'){
                    if ((strcmp(final,"/")==0)){
                        entrada.ninodo = reservar_inodo('d', permisos);
                        leer_inodo(entrada.ninodo,&inodo);
                        printf("[buscar_entrada()→ entrada.nombre: %s, entrada.ninodo: %d]\n",entrada.nombre,entrada.ninodo);
                        printf("[buscar_entrada()→ reservado inodo %d tipo %c con permisos %d]\n",entrada.ninodo,inodo.tipo,inodo.permisos);
                    } else {
                        printf("Error no existe directorio intermedio");
                        return -6; //Error no exsiste directorio intermedio
                    }
                } else {
                    entrada.ninodo = reservar_inodo('f', permisos);
                    leer_inodo(entrada.ninodo,&inodo);
                    printf("[buscar_entrada()→ entrada.nombre: %s, entrada.ninodo: %d]\n",entrada.nombre,entrada.ninodo);
                    printf("[buscar_entrada()→ reservado inodo %d tipo %c con permisos %d]\n",entrada.ninodo,inodo.tipo,inodo.permisos);
                }
                int escritos = mi_write_f(*p_inodo_dir, &entrada, num_entradas*sizeof(struct entrada), sizeof(struct entrada));
                if(escritos != sizeof(struct entrada)){
                    if (entrada.ninodo != -1){
                        liberar_inodo(entrada.ninodo);
                    }
                    printf("Error EXIT_FAILURE");
                    return EXIT_FAILURE; //Error EXIT_FAILURE
                }
            }
            break;
        }
 
    }
    if( (itipo == 'd' && strcmp(final, "/") == 0)||(itipo =='f' && strcmp(final, "\0")==0)){
        if ((nentrada < num_entradas)&&(reservar=1)) {
            printf("Error entrada ya exsistente");
            return -8; //Error entrada ya exsistente
        }
        *p_inodo = entrada.ninodo; //REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        *p_entrada = nentrada;//Quiza es sospechoso el *, añadido como parche
        printf("Final de la recursividad\n");
        return EXIT_SUCCESS; //Exit success
    } else {
        *p_inodo_dir = entrada.ninodo;//REVISAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAR
        return buscar_entrada(final, p_inodo_dir, p_inodo, p_entrada, reservar, permisos);
    }
}

int mi_creat(const char *camino, unsigned char permisos){
    printf("Llego a:mi creat\n");
    unsigned int p_entrada=0;
    unsigned int p_inodo_dir=0;
    unsigned int p_inodo=0; 
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,1,permisos);//Suponemos actualizacion de p_entrada
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
    unsigned int p_inodo;
    unsigned int p_inodo_dir =0;
    unsigned int p_entrada;
    struct inodo inodo;
    int nentradas;
    int offset = 0;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error == 1) {
        leer_inodo(p_inodo,&inodo);
        if (inodo.tipo=='d' && inodo.permisos&&4==4){
            nentradas = (inodo.tamEnBytesLog/sizeof(struct entrada))+1;
        }
        for(int i = 0; i < nentradas; i++){
            offset+= mi_read_f(p_inodo,buffer,offset,sizeof(struct entrada));
            buffer+='|';
        }
        return 0;
    }
    return -1;
}

int mi_chmod(const char *camino, unsigned char permisos){
    unsigned int p_inodo;
    unsigned int p_entrada;
    unsigned int p_inodo_dir =0;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error ==1) {
        mi_chmod_f(p_inodo,permisos);
        return 0;
    }
    return -1;
}

int mi_stat(const char *camino, struct STAT *p_stat){
    unsigned int p_inodo;
    unsigned int p_entrada;
    unsigned int p_inodo_dir =0;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error ==1) {
        mi_stat_f(p_inodo,p_stat);
        return 0;
    }
    return -1;
}

int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes){
    unsigned int p_inodo = 0;
    unsigned int p_entrada =0;
    unsigned int p_inodo_dir =0;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error ==1){
        printf("Llego a: inicio lectura");
        int bytesleidos= mi_read_f(p_inodo,buf,offset,nbytes);
        return bytesleidos;
    } else {
        printf("NO se ha encontrado entrada");
        return -1;
    }
}

int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes){
    unsigned int p_inodo = 0;
    unsigned int p_entrada =0;
    unsigned int p_inodo_dir =0;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error ==1){
        printf("Llego a: inicio lectura");
        int bytesleidos= mi_write_f(p_inodo,buf,offset,nbytes);
        return bytesleidos;
    } else {
        printf("NO se ha encontrado entrada");
        return -1;
    }
}

int mi_link(const char *camino1, const char *camino2){

    unsigned int p_inodo1 = 0;
    unsigned int p_entrada1 =0;
    unsigned int p_inodo_dir1 =0;

    unsigned int p_inodo2 = 0;
    unsigned int p_entrada2 =0;
    unsigned int p_inodo_dir2 =0;
    struct inodo inodo;
    struct entrada entrada;
    int error = buscar_entrada(camino1,&p_inodo_dir1,&p_inodo1,&p_entrada1,0,4);
    leer_inodo(p_inodo1,&inodo); 
    if (error!=1){
        printf("No existe camino1");
        return -1;
    }
    if (inodo.permisos&&4!=4){
        printf("No hay permisos de lectura");
        return -1;
    }
    error =buscar_entrada(camino2,&p_inodo_dir2,&p_inodo2,&p_entrada2,1,6);//Posible necesidad de nuevos p_inodo etc.
    if (error==-8){
        return -1;//Ruta de enlace ya existe;
    }
    mi_read_f(p_inodo_dir2, &entrada, p_entrada2*sizeof(struct  entrada), sizeof(struct entrada));
    entrada.ninodo= p_inodo1;
    mi_write_f(p_inodo_dir2, &entrada, p_entrada2*sizeof(struct entrada), sizeof(struct entrada));
    liberar_inodo(p_inodo2);//duda de si es este
    leer_inodo(p_inodo1,&inodo);
    inodo.nlinks++;
    inodo.ctime == time(NULL);
    escribir_inodo(p_inodo1,inodo);
    return 0;  
}

int mi_unlink(const char *camino){
    unsigned int p_inodo = 0;
    unsigned int p_entrada =0;
    unsigned int p_inodo_dir =0;
    struct inodo inodo;
    unsigned char *camino_aux=camino;
    int error = buscar_entrada(camino,&p_inodo_dir,&p_inodo,&p_entrada,0,4);
    if (error ==1){
        leer_inodo(p_inodo_dir,&inodo);
        int cont =0;
        while (cont <strlen(camino)-1){
            camino_aux++;
            cont ++;
        }
        if ((*camino_aux=='/')&&(inodo.tamEnBytesLog>0)){
            int nentradas = inodo.tamEnBytesLog/sizeof(struct entrada);
            if(p_entrada == nentradas - 1){
                mi_truncar_f(p_inodo_dir,inodo.tamEnBytesLog-sizeof(struct entrada));//qUIZA ES Pinodo
            } else {
                struct entrada entrada;
                mi_read_f(p_inodo_dir,);
            }
        }
    }
}