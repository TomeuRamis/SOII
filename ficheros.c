#include "ficheros.h"

int mi_write_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){
    int bytes_escritos = 0;
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);
    if (inodo.permisos&2==2){
        unsigned int BLinicio = offset/BLOCKSIZE;//Primer BL donde vamos a escribir
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE;//Ultimo bloque en que vamos a escribir
        //para la escritura del primer bloque
        int desp1 = offset%BLOCKSIZE;
        unsigned char buf_bloque[BLOCKSIZE];
        int BFinicio;
        //caso en que hay que leer el primer bloque
            BFinicio= traducir_bloque_inodo(ninodo,BLinicio,0);
            bread(BFinicio,buf_bloque);
            memcpy(buf_bloque+desp1,buf_original,BLOCKSIZE-desp1);
            bwrite(BFinicio,buf_bloque);
            bytes_escritos = BLOCKSIZE-desp1;
        int BF;
        //bloques intermedios se escriben enteros
        for(int i =BLinicio+1;i < BLfinal;i++){
           BF= traducir_bloque_inodo(ninodo,i,0);
           bwrite (BF, buf_original + (BLOCKSIZE - desp1) + (i - BLinicio - 1) * BLOCKSIZE);
           bytes_escritos += BLOCKSIZE; 
        }
        //se escribe el ultimo bloque
        BF= traducir_bloque_inodo(ninodo,BLfinal,0);
        bread(BF,buf_bloque);
        int desp2= (BLinicio+nbytes-1)%BLOCKSIZE;//¬¬ suspicius
        memcpy (buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
        bwrite(BF,buf_bloque);
        bytes_escritos += desp2+1;
        if (offset+nbytes>inodo.tamEnBytesLog){
            inodo.tamEnBytesLog = offset+nbytes;
            inodo.ctime = time(NULL);
        }
        inodo.mtime = time(NULL);
        escribir_inodo(ninodo,inodo);
    }  
    return bytes_escritos;
}

int mi_read_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){
    int leidos = 0;
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    if (inodo.permisos&4==4){   //Comprobar que tenemos permisos para leer
        if (offset>= inodo.tamEnBytesLog){  //El offset se sale del tamaño del inodo
            return leidos;
        }
        if (offset+nbytes >= inodo.tamEnBytesLog){  //Acortamos la longitud a escribir para que quepa en el inodo.
            nbytes = inodo.tamEnBytesLog-offset;
        }
        
        unsigned int BLinicio = offset/BLOCKSIZE;//Primer BL donde vamos a leer
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE;//Ultimo bloque que vamos a leer
        int desp1 = offset%BLOCKSIZE;
        
        unsigned char buf_bloque[BLOCKSIZE];
        int BFinicio;
        //caso en que hay que leer el primer bloque
        BFinicio= traducir_bloque_inodo(ninodo,BLinicio,0);
        if (BFinicio != -1){
            bread(BFinicio,buf_bloque);
            memcpy(buf_original,buf_bloque+desp1,BLOCKSIZE-desp1);
        }
        leidos+=BLOCKSIZE-desp1;
        
         int BF;
        //bloques intermedios se leen enteros
        for(int i =BLinicio+1;i < BLfinal;i++){
            BF= traducir_bloque_inodo(ninodo,i,0);
            if (BF != -1){
                bread(BF, buf_bloque);
                memcopy(buf_original+leidos, buf_bloque, BLOCKSIZE);
            }
        leidos += BLOCKSIZE; 
        }
         //se escribe el ultimo bloque
        int desp2= (BLinicio+nbytes-1)%BLOCKSIZE;//¬¬ suspicius
        BF= traducir_bloque_inodo(ninodo,BLfinal,0);
        if (BF != -1){
            bread(BF,buf_bloque);          
            memcpy (buf_original, buf_bloque + (nbytes - desp2 - 1), desp2 + 1);
        }
        leidos += desp2+1;

    }
    return leidos;
}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat){
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    p_stat ->tipo = inodo.tipo;
    p_stat ->permisos = inodo.permisos;
    p_stat ->atime = inodo.atime;
    p_stat ->mtime = inodo.mtime;
    p_stat ->ctime = inodo.ctime;
    p_stat ->nlinks = inodo.nlinks;
    p_stat ->tamEnBytesLog = inodo.tamEnBytesLog;
    p_stat ->numBloquesOcupados = inodo.numBloquesOcupados;
    return 0;
}

int mi_chmod_f(unsigned int ninodo, unsigned char permisos){
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    inodo.permisos=permisos;
    inodo.ctime=time(NULL);
    escribir_inodo(ninodo,inodo);
    return 0;
}

int mi_truncar_f(unsigned int ninodo,unsigned int nbytes){
    int liberados = 0;
    unsigned int nblogico;
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    if (inodo.permisos&2==2){
        if (nbytes >= inodo.tamEnBytesLog){ //No se puede truncar un archivo a un tamaño superior
            fprintf(stderr, "ERROR: el tamaño a truncar es demasiado grande.");
        } else{
            if(nbytes%BLOCKSIZE == 0){
                nblogico = nbytes/BLOCKSIZE;
            } else{
                nblogico = nbytes/BLOCKSIZE +1;
            }
            liberados = liberar_bloques_inodo(ninodo, nblogico);
            inodo.mtime = time(NULL);
            inodo.ctime = time(NULL);
            inodo.tamEnBytesLog = nbytes;
            escribir_inodo(ninodo,inodo);
        }   
    }
    return liberados;
}