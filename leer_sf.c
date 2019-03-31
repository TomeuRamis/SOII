#include "ficheros_basico.h"

int main(int argc, char **argv){

    bmount(argv[1]);
    bread(0, &SB);
    
    printf("Posición posPrimerBloqueMB: %d\n", SB.posPrimerBloqueMB);
    printf("Posición posUltimoBloqueMB: %d\n", SB.posUltimoBloqueMB);
    printf("Posición posPrimerBloqueAI: %d\n", SB.posPrimerBloqueAI);
    printf("Posición posUltimoBloqueAI: %d\n", SB.posUltimoBloqueAI);
    printf("Posición posPrimerBloqueDatos: %d\n", SB.posPrimerBloqueDatos);
    printf("Posición posUltimoBloqueDatos: %d\n", SB.posUltimoBloqueDatos);
    printf("Posición posInodoRaiz: %d\n", SB.posInodoRaiz);
    printf("Posición posPrimerInodoLibre: %d\n", SB.posPrimerInodoLibre);
    printf("Posición cantBloquesLibres: %d\n", SB.cantBloquesLibres);
    printf("Posición cantInodosLibres: %d\n", SB.cantInodosLibres);
    printf("Posición totBloques: %d\n", SB.totBloques);
    printf("Posición totInodos: %d\n", SB.totInodos);
    printf("Tamaño struct superbloque: %lu\n", sizeof(struct superbloque));
    printf("Tamaño struct inodo: %lu\n", sizeof(struct inodo));

    reservar_bloque();

    printf("Cantidad bloques libres: %d \n", SB.cantBloquesLibres);

    liberar_bloque(SB.posPrimerBloqueDatos);

    printf("Cantidad bloques libres: %d \n", SB.cantBloquesLibres);

    unsigned char bit = leer_bit(0);
    printf("Bloque 0 --> %d\n",bit);
    bit = leer_bit(1);
    printf("Bloque 1 --> %d\n",bit);
    bit = leer_bit(13);
    printf("Bloque 13 --> %d\n",bit);
    bit = leer_bit(14);
    printf("Bloque 14 --> %d\n",bit);
    bit = leer_bit(3138);
    printf("Bloque 3138 --> %d\n",bit);
    bit = leer_bit(3139);
    printf("Bloque 3139 --> %d\n",bit);
    bit = leer_bit(99999);
    printf("Bloque 99999 --> %d\n",bit);

    // struct tm *ts;
    // char atime[80];
    // char mtime[80];
    // char ctime[80];
    // struct inodo inodo;
    // int ninodo = 0;
    // leer_inodo(ninodo,&inodo);
    // printf("tipo: %s\n",&inodo.tipo);
    // printf("permisos: %d\n",inodo.permisos);
    // ts = localtime(&inodo.atime);
    // strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    // ts = localtime(&inodo.mtime);
    // strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    // ts = localtime(&inodo.ctime);
    // strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    // printf("ID: %d\nATIME: %s\nMTIME: %s\nCTIME: %s\n",ninodo,atime,mtime,ctime);
    // printf("nLinks: %d\n",inodo.nlinks);
    // printf("tamaño en bytes: %d\n",inodo.tamEnBytesLog);
    // printf("numero de bloques ocupados: %d\n\n\n",inodo.numBloquesOcupados); 
    
    //Pruebas nievl 4
    printf("PRUEBAS NIVEL 4\n\n");
    int nInodo= reservar_inodo('f',6);
    traducir_bloque_inodo(nInodo,8,1);
    traducir_bloque_inodo(nInodo,204,1);
    traducir_bloque_inodo(nInodo,30004,1);
    traducir_bloque_inodo(nInodo,400004,1);
    traducir_bloque_inodo(nInodo,16843019,1);

    struct tm *ts;
    char atime[80];
    char mtime[80];
    char ctime[80];
    struct inodo inodo;
    leer_inodo(nInodo,&inodo);
    printf("tipo: %s\n",&inodo.tipo);
    printf("permisos: %d\n",inodo.permisos);
    ts = localtime(&inodo.atime);
    strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.mtime);
    strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
    ts = localtime(&inodo.ctime);
    strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
    printf("ID: %d\nATIME: %s\nMTIME: %s\nCTIME: %s\n",nInodo,atime,mtime,ctime);
    printf("nLinks: %d\n",inodo.nlinks);
    printf("tamaño en bytes: %d\n",inodo.tamEnBytesLog);
    printf("numero de bloques ocupados: %d\n\n\n",inodo.numBloquesOcupados); 
    

    //pruebas nivel 5
        struct inodo inod;
    leer_inodo(nInodo, &inod);
    printf("Numero de bloques ocupados: %d\n", inod.numBloquesOcupados);
    printf("Posición del primer inodo libre: %d\n", SB.posPrimerInodoLibre);
    liberar_inodo(nInodo);
    
    bumount();
    return 0;
}

