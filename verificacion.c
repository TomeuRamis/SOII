#include "verificacion.h"
int cant_registros = 256;
int main(int argc, char **argv){
    if (argc == 3){
        bmount(argv[1]);
        struct STAT stat;
        
        mi_stat(argv[2],&stat);
        int nentrada = stat.tamEnBytesLog/sizeof(struct entrada);
        if(nentrada!=NUMPROCESOS){
            return -1;
        }
        char fichero[100];
        char directorio_padre[100];
        strcpy(directorio_padre,argv[2]);
        sprintf(fichero, "%sinforme.txt",directorio_padre);
        mi_creat(fichero, 6);
        struct entrada entrada;
        
        for(int i = 0; i < nentrada; i++){
            struct INFORMACION info;
            //struct REGISTRO buf;
            struct REGISTRO big_buffer[TAM_BIG_BUF];
            memset(big_buffer, 0, sizeof(big_buffer));
            mi_read(argv[2], &entrada, i*sizeof(struct entrada), sizeof(struct entrada));
            
            char *entr = entrada.nombre;
            pid_t pid = atoi(strchr(entr,'_')+1);
            
            char camino_prueba[100];
            sprintf(camino_prueba, "%s%s/prueba.dat", directorio_padre, entrada.nombre);

            int escrituras_validadas = 0;
            int pos=0;
            while( mi_read(camino_prueba,&big_buffer,pos*sizeof(struct REGISTRO),sizeof(struct REGISTRO)*TAM_BIG_BUF)> 0){
                for(int j = 0; j < TAM_BIG_BUF; j++){
                    if(pid==big_buffer[j].pid){
                        if(escrituras_validadas == 0){
                            info.MenorPosicion=big_buffer[j];
                            info.MayorPosicion=big_buffer[j];
                            info.PrimeraEscritura=big_buffer[j];
                            info.UltimaEscritura=big_buffer[j];
                        }
                        else{
                            if(difftime(info.UltimaEscritura.fecha/*.time*/, big_buffer[j].fecha/*.time*/) < 0){ //Comparamos la fecha
                                info.UltimaEscritura = big_buffer[j];
                            } else if(difftime(info.UltimaEscritura.fecha/*.time*/, big_buffer[j].fecha/*.time*/)==0){
                                /*if((info.UltimaEscritura.fecha.millitm - big_buffer[j].fecha.millitm) < 0){ //Comparamos los milisegundos
                                    info.UltimaEscritura = big_buffer[j];
                                } else if ((info.UltimaEscritura.fecha.millitm - big_buffer[j].fecha.millitm) == 0){*/
                                    if(info.UltimaEscritura.nEscritura < big_buffer[j].nEscritura) {
                                        info.UltimaEscritura = big_buffer[j];
                                    }
                                /*}*/
                            }
                            if(difftime(info.PrimeraEscritura.fecha/*.time*/, big_buffer[j].fecha/*.time*/) > 0){
                                info.PrimeraEscritura=big_buffer[j];
                            } else if(difftime(info.PrimeraEscritura.fecha/*.time*/, big_buffer[j].fecha/*.time*/)==0){
                                /*if((info.PrimeraEscritura.fecha.millitm - big_buffer[j].fecha.millitm) > 0){
                                    info.PrimeraEscritura=big_buffer[j];
                                } else if((info.PrimeraEscritura.fecha.millitm - big_buffer[j].fecha.millitm) == 0){*/
                                    if(info.PrimeraEscritura.nEscritura > big_buffer[j].nEscritura) {
                                        info.PrimeraEscritura = big_buffer[j];
                                    }
                                /*}*/
                            }
                        }
                        info.MayorPosicion = big_buffer[j];
                        escrituras_validadas++;
                    }
                    pos++;  
                } 
                memset(big_buffer, 0, sizeof(big_buffer));
            }
            info.nEscrituras=escrituras_validadas;
            info.pid=pid;
            printf("%d) %d escrituras validadas en %s\n", i, escrituras_validadas, camino_prueba);
            char aux[1000];
            sprintf(aux, "PID: %d\nNumero escrituras: %d\nPrimera escritura\t%d\t%d %sUltima escritura\t%d\t%d %sMenor escritura         %d\t%d %sMayor escritura         %d\t%d %s\n",
            info.pid,info.nEscrituras,info.PrimeraEscritura.nEscritura, info.PrimeraEscritura.nRegistro,asctime(localtime(&info.PrimeraEscritura.fecha/*.time*/)),/*info.PrimeraEscritura.fecha.millitm,*/ 
            info.UltimaEscritura.nEscritura, info.UltimaEscritura.nRegistro, asctime(localtime(&info.UltimaEscritura.fecha/*.time*/)),/*info.PrimeraEscritura.fecha.millitm,*/ info.MenorPosicion.nEscritura, 
            info.MenorPosicion.nRegistro, asctime(localtime(&info.MenorPosicion.fecha/*.time*/)),/*info.MenorPosicion.fecha.millitm,*/ info.MayorPosicion.nEscritura, info.MayorPosicion.nRegistro, 
            asctime(localtime(&info.MayorPosicion.fecha/*.time*/))/*, info.MayorPosicion.fecha.millitm*/);

            mi_write(fichero, &aux, i*sizeof(aux), strlen(aux));
        }
        bumount();
    }else{
        printf("Sintaxis: ./verificacion <disco> <directorio_simulacion>\n");
    }
    return 0;
}