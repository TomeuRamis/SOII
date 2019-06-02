#include "verificacion.h"
int cant_registros = 256;
int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        bread(0,&SB);
        struct STAT stat;
        
        mi_stat(argv[2],&stat);
        int nentrada = stat.tamEnBytesLog/sizeof(struct entrada);
        if(nentrada!=NUMPROCESOS){
            return -1;
        }
        char *fichero = argv[2];
        strcat(fichero, "informe.txt");
        mi_creat(fichero, 6);
        struct entrada entrada;
        for(int i = 0; i < nentrada; i++){
            struct INFORMACION info;
            mi_read(argv[2], &entrada, i*sizeof(struct entrada), sizeof(struct entrada));
            char *pid = entrada.nombre;
            pid = pid + 8;
            pid[6] = '\0';
            //struct REGISTRO buffer[cant_registros];
            struct REGISTRO buf;
            char *camino_prueba;
            strcat(camino_prueba, argv[2]);
            strcat(camino_prueba, entrada.nombre);
            strcat(camino_prueba,"prueba.dat");
            int escrituras_validadas = 0;
            while( mi_read(camino_prueba,&buf,0,sizeof(struct REGISTRO))> 0){
                if(strcmp(pid, buf.pid) == 0){
                    if(escrituras_validadas == 0){
                        info.MenorPosicion=buf;
                        info.MayorPosicion=buf;
                        info.PrimeraEscritura=buf;
                        info.UltimaEscritura=buf;
                    }
                    else{
                        if(difftime(info.UltimaEscritura.fecha, buf.fecha) < 0){
                            info.UltimaEscritura = buf;
                        } else if(difftime(info.UltimaEscritura.fecha, buf.fecha)==0){
                            if(info.UltimaEscritura.nEscritura < buf.nEscritura) {
                                info.UltimaEscritura = buf;
                            }
                        }
                        if(difftime(info.PrimeraEscritura.fecha, buf.fecha) > 0){
                            info.PrimeraEscritura=buf;
                        } else if(difftime(info.PrimeraEscritura.fecha, buf.fecha)==0){
                            if(info.PrimeraEscritura.nEscritura > buf.nEscritura) {
                                info.PrimeraEscritura = buf;
                            }
                        }
                    }
                    info.MayorPosicion = buf;
                    escrituras_validadas++;
                }   
            }
            info.nEscrituras=escrituras_validadas;
            printf("%d) %d escrituras validadas en %s", i, escrituras_validadas, camino_prueba);
            char *aux = malloc(100000000000);
            sprintf(aux, "\nPID: %d\nNumero escrituras: %d\nPrimera escritura\t%d\t%d %s\nUltima escritura\t%d\t%d %d\nMenor escritura\t%d\t%d %d\nMayor escritura\t%d\t%d %d\n",info.pid,info.nEscrituras,info.PrimeraEscritura.nEscritura, info.PrimeraEscritura.nRegistro,asctime(localtime(&info.PrimeraEscritura.fecha)), info.UltimaEscritura.nEscritura, info.UltimaEscritura.nRegistro, asctime(localtime(&info.UltimaEscritura.fecha)), info.MenorPosicion.nEscritura, info.MenorPosicion.nRegistro, asctime(localtime(&info.MenorPosicion.fecha)), info.MayorPosicion.nEscritura, info.MayorPosicion.nRegistro, asctime(localtime(&info.MayorPosicion.fecha)));

            mi_write(fichero, aux, i*sizeof(&aux), sizeof(struct INFORMACION));
        }
        bumount();
    }else{
        printf("Sintaxis: ./verificacion <disco> <directorio_simulacion>\n");
    }
    return 0;
}