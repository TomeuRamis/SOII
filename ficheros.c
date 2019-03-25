#include "ficheros_basico.h"

int mi_write_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){
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
            bwrite(BFinicio,buf_original);
        int BF;
        //bloques intermedios se escriben enteros
        for(int i =BLinicio+1;i < BLfinal;i++){
           BF= traducir_bloque_inodo(ninodo,i,0);
           bwrite (BF, buf_original + (BLOCKSIZE - desp1) + (i - BLinicio - 1) * BLOCKSIZE);

        }
        //se escribe el ultimo bloque
        BF= traducir_bloque_inodo(ninodo,BLfinal,0);
        bread(BF,buf_bloque);
        int desp2= (BLinicio+nbytes-1)%BLOCKSIZE;//¬¬ suspicius
        memcpy (buf_bloque, buf_original + (nbytes - desp2 - 1), desp2 + 1);
        bwrite(BF,buf_bloque);
        if (offset+nbytes>inodo.tamEnBytesLog){
            inodo.tamEnBytesLog = offset+nbytes;
            inodo.ctime = time(NULL);
        }
        inodo.mtime = time(NULL);
        escribir_inodo(ninodo,inodo);
    }     
}

int mi_read_f(unsigned int ninodo,const void *buf_original, unsigned int offset, unsigned int nbytes){

}

int mi_stat_f(unsigned int ninodo, struct STAT *p_stat){

}

int mi_chmod_f(unsigned int ninodo, unsigned char permisos){

}

int mi_truncar_f(unsigned int ninodo,unsigned int nbytes){

}