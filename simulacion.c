#include "simulacion.h"
#include <stdlib.h> 

static int acabados =0;

void reaper(){
  pid_t ended;
  signal(SIGCHLD, reaper);
  while ((ended=waitpid(-1, NULL, WNOHANG))>0) {
    acabados++;
   //Podemos testear qué procesos van acabando:
    //fprintf(stderr, "acabado: %d total acabados: %d\n", ended, acabados);
  }
}

int main (int argc, char **argv){
    if (argc == 2){
      bmount(argv[1]);
      bread(0, &SB);
      char camino[100];
      memset(camino, 0, 100);
      time_t now = time (0);
      strftime (camino, 100, "/simul_%Y%m%d%H%M%S/", localtime (&now));
      printf ("Directorio: %s\n", camino);

      mi_creat(camino,6);

      signal(SIGCHLD, reaper);
      for(int i = 1; i <= NUMPROCESOS; i++){
        int pid = fork();
        char camino_aux[100];
        memset(camino_aux, 0, 100);
        strcat(camino_aux, camino);
        if(pid > 0){ //Es el hijo
          bmount(argv[1]);
          strcat(camino_aux, "proceso_");
          char spid[10];
          sprintf(spid, "%d", pid);
          strcat(camino_aux, spid);
          strcat(camino_aux, "/");
          mi_creat(camino_aux, 6);
          printf("Proceso %d: Creado directorio %s\n", i, camino_aux);          
          char *fichero = "prueba.dat";
          strcat(camino_aux, fichero);
          int er = mi_creat(camino_aux,7);
          if(er == 1){
            printf("proceso %d: creado archivo.dat chacho %s\n", i, camino_aux);
          }
          srand(time(NULL)+getpid());
          for(int j = 0; j < 50; j++){
            struct REGISTRO reg;
            reg.fecha=time(NULL);
            reg.pid=getpid();
            reg.nEscritura=j+1;
            reg.nRegistro=rand()%REGMAX;
            
            int erroro = mi_write(camino_aux, &reg, reg.nRegistro*sizeof(struct REGISTRO), sizeof(struct REGISTRO));
            
            if (erroro != 1){
              printf("Proceso %d: error %d\n", i, erroro);
              printf("Proceso %d: camino escrito: %s\n", i, camino_aux);
            }
            sleep(0.05);
          }
          printf("Proceso %d: Completadas las 50 escrituras en %s\n", i, camino_aux);  
          bumount();
          exit(0);
        }
        sleep(0.2);
      }
      while(acabados< NUMPROCESOS){
        pause();
      }
      bumount();
    }else{
        printf("Sintaxis: ./simulacion <disco>\n");
    }
    return 0;
}

