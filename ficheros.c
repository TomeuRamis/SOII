#include "ficheros.h"

int mi_write_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){
    struct inodo inodo;
    int escritos = 0;
    leer_inodo(ninodo,&inodo);
    if (inodo.permisos&&2==2){
        unsigned int BLinicio = offset/BLOCKSIZE;//Primer BL donde vamos a escribir
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE;//Ultimo bloque en que vamos a escribir
        int desp1 = offset%BLOCKSIZE;
        unsigned char buf_bloque[BLOCKSIZE];
        int BF;
        
        
        memset(buf_bloque,0,BLOCKSIZE);
        BF= traducir_bloque_inodo(ninodo,BLinicio,1);
        //Caso en que el texto cabe en un solo bloque
        if (nbytes <=BLOCKSIZE-desp1 && BF!=-1){           
            memcpy(buf_bloque+desp1,buf_original,nbytes);
            bwrite(BF,buf_bloque);   
            escritos += nbytes;     
        } else{
            //caso en que hay que leer el primer bloque
            bread(BF,buf_bloque);
            memcpy(buf_bloque+desp1,buf_original,BLOCKSIZE-desp1); 
            bwrite(BF,buf_bloque);
            escritos += BLOCKSIZE-desp1;
            //Se escriben los bloques intermedios
            for(int i =BLinicio+1;i < BLfinal;i++){
                memset(buf_bloque,0,BLOCKSIZE);
                BF= traducir_bloque_inodo(ninodo,i,1); 
                if (BF!= -1){
                    memcpy(buf_bloque,buf_original + (BLOCKSIZE - desp1) + (i - BLinicio - 1) * BLOCKSIZE,BLOCKSIZE);          
                    bwrite (BF,buf_bloque);
                    escritos+= BLOCKSIZE;
                }
            }
            //se escribe el ultimo bloque
            BF= traducir_bloque_inodo(ninodo,BLfinal,1);
            if (BF!=-1 && BLinicio!=BLfinal){  //Si se puede reservar el bloque y es diferente al inicial
                bread(BF,buf_bloque);
                int desp2= (offset+nbytes-1)%BLOCKSIZE;
                memset(buf_bloque,0,BLOCKSIZE);
                memcpy (buf_bloque,buf_original + (nbytes - desp2 - 1), desp2 + 1);
                bwrite(BF,buf_bloque);
                escritos += desp2+1;
            }
        }
        leer_inodo(ninodo,&inodo);
        if (offset+nbytes>inodo.tamEnBytesLog){
            inodo.tamEnBytesLog = offset+nbytes;
            inodo.ctime = time(NULL);
        }
        inodo.mtime = time(NULL);
        escribir_inodo(ninodo,inodo);
    }
    return escritos;
}

int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes){
    int leidos = 0;
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    if (inodo.permisos&&4==4){              //Comprobar que tenemos permisos para leer
        //printf("Offset: %d\n", offset);
        if (offset>= inodo.tamEnBytesLog){  //El offset se sale del tamaño del inodo
            fprintf(stderr,"El offset se sale del tamaño del inodo\n");      
            return leidos;
        }
        if (offset+nbytes >= inodo.tamEnBytesLog){  //Acortamos la longitud a escribir para que quepa en el inodo.
            nbytes = inodo.tamEnBytesLog-offset;
        }
        
        unsigned int BLinicio = offset/BLOCKSIZE;           //Primer BL donde vamos a leer
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE; //Ultimo bloque que vamos a leer    
        int desp1 = offset%BLOCKSIZE;
        
        unsigned char buf_bloque[BLOCKSIZE];
        memset(buf_bloque, 0, BLOCKSIZE);
        memset(buf_original, 0, BLOCKSIZE);
        int BF;
        //caso en que hay que leer el primer bloque
        BF= traducir_bloque_inodo(ninodo,BLinicio,0);
        if(BF == -1){   //Si el bloque no existe, devolvemos los bytes que deberiamos haber leido
            return nbytes;
        } else {
            if (nbytes <=BLOCKSIZE-desp1){           
                bread(BF,buf_bloque);
                memcpy(buf_original,buf_bloque+desp1,nbytes); 
                leidos += nbytes;  
                printf("eSTOY EN LA PRIMERA%d\n", leidos);
                return leidos;
            } else { 
                printf("Estoy en la segunda%d\n", leidos);
                bread(BF,buf_bloque);
                memcpy(buf_original,buf_bloque+desp1,BLOCKSIZE-desp1);
                leidos+=BLOCKSIZE-desp1;    
                //bloques intermedios se leen enteros
                for(int i =BLinicio+1;i < BLfinal;i++){
                    BF= traducir_bloque_inodo(ninodo,i,0);
                    if (BF != -1){
                        bread(BF, buf_bloque);
                        memcpy(buf_original+leidos, buf_bloque, BLOCKSIZE);
                        
                    }  
                    leidos += BLOCKSIZE;  
                }
                //se escribe el ultimo bloque
                int desp2= (offset+nbytes-1)%BLOCKSIZE;
                if(BLinicio!=BLfinal){
                    BF= traducir_bloque_inodo(ninodo,BLfinal,0);
                    if (BF != -1){ //Que exsista el bloque y no sea el mismo del principio
                        memset(buf_bloque,0,BLOCKSIZE);
                        bread(BF,buf_bloque); 
                        memcpy (buf_original+leidos, buf_bloque, desp2 + 1); 
                    }
                    leidos += desp2+1;
                }
            }
       }
    } else {
        printf("No hay permisos de lectura\n");
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
    if (inodo.permisos&&2==2){
        if (nbytes >= inodo.tamEnBytesLog){         //No se puede truncar un archivo a un tamaño superior
            fprintf(stderr, "ERROR: el tamaño a truncar es demasiado grande.\n");
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
            inodo.numBloquesOcupados = inodo.numBloquesOcupados-liberados;
            escribir_inodo(ninodo,inodo);
        }   
    } else{
        fprintf(stderr, "No hay permisos de escritura\n");
    }
    return liberados;
}