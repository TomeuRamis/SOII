#include "directorios.h"

int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        bread(0, &SB);
        struct STAT stat;
        printf("Sizeof struct: %d",sizeof(struct STAT));
        mi_stat(argv[2],&stat);

        struct tm *ts;
        char atime[80];
        char mtime[80];
        char ctime[80];
        printf("tipo: %s\n",stat.tipo);
        printf("permisos: %d\n",stat.permisos);
        ts = localtime(stat.atime);
        strftime(atime, sizeof(atime), "%a %Y-%m-%d %H:%M:%S", ts);
        ts = localtime(stat.mtime);
        strftime(mtime, sizeof(mtime), "%a %Y-%m-%d %H:%M:%S", ts);
        ts = localtime(stat.ctime);
        strftime(ctime, sizeof(ctime), "%a %Y-%m-%d %H:%M:%S", ts);
        printf("ATIME: %s\nMTIME: %s\nCTIME: %s\n",atime,mtime,ctime);
        printf("nLinks: %d\n",stat.nlinks);
        printf("tamaño en bytes: %d\n",stat.tamEnBytesLog);
        printf("numero de bloques ocupados: %d\n\n\n",stat.numBloquesOcupados); 
        bumount();
    } else {
        printf("Sintaxi: ./mi_stat <disco> </ruta>");
    }
}