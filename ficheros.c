#include "ficheros.h"

int mi_write_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){
    //int bytes_escritos = 0;
    struct inodo inodo;
    leer_inodo(ninodo,&inodo);
    if (inodo.permisos&&2==2){
        unsigned int BLinicio = offset/BLOCKSIZE;//Primer BL donde vamos a escribir
        //printf("BLinicio: %d\n", BLinicio);
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE;//Ultimo bloque en que vamos a escribir
        //printf("BLfinal: %d\n", BLfinal);
        //para la escritura del primer bloque
        int desp1 = offset%BLOCKSIZE;
        //printf("desp1: %d\n", desp1);
        unsigned char buf_bloque[BLOCKSIZE];
        int BF;
        
        //caso en que hay que leer el primer bloque
        memset(buf_bloque,0,BLOCKSIZE);
        BF= traducir_bloque_inodo(ninodo,BLinicio,1);
        if (nbytes <=BLOCKSIZE-desp1 && BF!=-1){           
            memcpy(buf_bloque+desp1,buf_original,nbytes); //He quitado ampersan
            //printf("BUf_bloque: %s",buf_bloque+desp1);
            bwrite(BF,buf_bloque);        
        } else{
            bread(BF,buf_bloque);
            memcpy(buf_bloque+desp1,buf_original,BLOCKSIZE-desp1); //He quitado ampersan
            //printf("BUf_bloque: %s",buf_bloque+desp1);
            bwrite(BF,buf_bloque);
            //printf("BUf_bloque: %s",buf_bloque);
            //bytes_escritos = BLOCKSIZE-desp1;        
            //bloques intermedios se escriben enteros
            for(int i =BLinicio+1;i < BLfinal;i++){
                memset(buf_bloque,0,BLOCKSIZE);
                BF= traducir_bloque_inodo(ninodo,i,1); 
                if (BF!= -1){
                    //bread(BF,buf_bloque);       //Habia BFinicio
                    memcpy(buf_bloque,buf_original + (BLOCKSIZE - desp1) + (i - BLinicio - 1) * BLOCKSIZE,BLOCKSIZE);          
                    bwrite (BF,buf_bloque);
                    //printf("BUf_bloque: %s",buf_bloque);
                    //bytes_escritos += BLOCKSIZE; 
                }
            }
            //se escribe el ultimo bloque
            BF= traducir_bloque_inodo(ninodo,BLfinal,1);
            if (BF!=-1 && BLinicio!=BLfinal){  //Si se puede reservar el bloque y es diferente al inicial
                bread(BF,buf_bloque);
                int desp2= (offset+nbytes-1)%BLOCKSIZE;//¬¬ suspicius
                memset(buf_bloque,0,BLOCKSIZE);
                memcpy (buf_bloque,buf_original + (nbytes - desp2 - 1), desp2 + 1);
                bwrite(BF,buf_bloque);
                //printf("BUf_bloque: %s",buf_bloque);
                //bytes_escritos += desp2+1;
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
    //printf("\nBytes escritos en mi_write_f() = %d",bytes_escritos);  
    return nbytes;
}

int mi_read_f(unsigned int ninodo, void *buf_original, unsigned int offset, unsigned int nbytes){
    int leidos = 0;
    struct inodo inodo;
    leer_inodo(ninodo, &inodo);
    if (inodo.permisos&&4==4){   //Comprobar que tenemos permisos para leer
        if (offset>= inodo.tamEnBytesLog){  //El offset se sale del tamaño del inodo
            printf("El offset se sale del tamaño del inodo\n");
            return leidos;
        }
        if (offset+nbytes >= inodo.tamEnBytesLog){  //Acortamos la longitud a escribir para que quepa en el inodo.
            nbytes = inodo.tamEnBytesLog-offset;
        }
        
        unsigned int BLinicio = offset/BLOCKSIZE;//Primer BL donde vamos a leer
        unsigned int BLfinal = (offset+nbytes-1)/BLOCKSIZE;//Ultimo bloque que vamos a leer    
        int desp1 = offset%BLOCKSIZE;
        //printf("desp1  %d\n", desp1);
        
        unsigned char buf_bloque[BLOCKSIZE];
        memset(buf_bloque, 0, BLOCKSIZE);
        memset(buf_original, 0, BLOCKSIZE);
        int BF;
        //caso en que hay que leer el primer bloque
        BF= traducir_bloque_inodo(ninodo,BLinicio,0);
        //printf("BFinicio: %d\n", BF);
/*         if (nbytes <=BLOCKSIZE-desp1 && BF!=-1){
            bread(BF,buf_bloque);
            memcpy(buf_original,buf_bloque+desp1,nbytes); //He quitado ampersan
            return nbytes;
        } */
        if (BF!= -1){
            bread(BF,buf_bloque);
            //printf("Primer bloque leido: %s\n\n", buf_bloque+desp1);   
            memcpy(buf_original,buf_bloque+desp1,BLOCKSIZE-desp1);
            //printf("El buffer original ahora contiene: %s\n",buf_original );
            leidos+=BLOCKSIZE-desp1;
        }      
        //printf("leidos 1: %d\n", leidos);
        //bloques intermedios se leen enteros
        //printf("Valor BLinicio antes del bucle mi_read_f(): %d\n", BLinicio);
        for(int i =BLinicio+1;i < BLfinal;i++){
            BF= traducir_bloque_inodo(ninodo,i,0);
            //printf("Valor BF en bucle mi_read_f: %d\n",BF);
            if (BF != -1){
                bread(BF, buf_bloque);
                //printf("Siguiente bloque leido: %s\n\n", buf_bloque+desp1);   
                memcpy(buf_original+leidos, buf_bloque, BLOCKSIZE);
                //printf("El buffer original ahora contiene: %s\n",buf_original );
                leidos += BLOCKSIZE;
            }    
        }
         //se escribe el ultimo bloque
        int desp2= (offset+nbytes-1)%BLOCKSIZE;//¬¬ suspicius
        if(BLinicio!=BLfinal){
            BF= traducir_bloque_inodo(ninodo,BLfinal,0);
            if (BF != -1){ //Que exsista el bloque y no sea el mismo del principio
                memset(buf_bloque,0,BLOCKSIZE);
                bread(BF,buf_bloque); 
                //printf("Ultimo bloque leido: %s\n\n", buf_bloque+desp1);   
                memcpy (buf_original+leidos, buf_bloque, desp2 + 1);
                //printf("El buffer original ahora contiene: %s\n",buf_original );
                leidos += desp2+1;
            }
        }
        //printf("leidos 2: %d\n", leidos);

    } else {
        printf("No hay permisos de lectura\n");
    }
    //printf("\nbuf_original = %ls\n",buf_original);
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
            inodo.numBloquesOcupados = inodo.numBloquesOcupados-liberados;
            escribir_inodo(ninodo,inodo);
        }   
    }
    return liberados;
}