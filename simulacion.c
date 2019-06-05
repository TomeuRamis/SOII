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
      char camino[100];
      memset(camino, '\0', 100);
      time_t now = time (0);
      strftime (camino, 100, "/simul_%Y%m%d%H%M%S/", localtime (&now));
      printf ("Directorio: %s\n", camino);

      mi_creat(camino,6);
      pid_t pid;
      signal(SIGCHLD, reaper);
      for(int i = 1; i <= NUMPROCESOS; i++){
        pid = fork();  
        if(pid == 0){ //Es el hijo

          bmount(argv[1]);

          char camino_aux[100];
          memset(camino_aux, 0, 100);
          //char *camino_aux=malloc(sizeof(char));
          memset(camino_aux, '\0', 100);
          strcat(camino_aux, camino);
          strcat(camino_aux, "proceso_");
          char spid[10];
          sprintf(spid, "%d", getpid());
          strcat(camino_aux, spid);
          strcat(camino_aux, "/");
          //printf("camino_aux: %s\n", camino_aux);

          mi_creat(camino_aux, 6);       

          //char *fichero = "prueba.dat";
          char final[100];
          memset(final, '\0', 100);
          strcat(final,camino_aux);
          strcat(final, "prueba.dat");
          //printf("final: %s\n", final);

          int er = mi_creat(final,6);
          if(er != 1){
            bumount();
            exit(0);
          } else{
            // printf("Proceso %d: la creacion del archivo es correcta\n", i);
          }

          srand(time(NULL)+getpid()); 
          for(int j = 0; j < NUM_ESCRITURAS; j++){
            struct REGISTRO reg;
            reg.fecha=time(NULL);
            //ALTERNATIVA
            /*struct timeb tm;  
            ftime(&tm);
            reg.fecha = tm;*/
            //FN ALTERNATIVA
            reg.pid=getpid();
            reg.nEscritura=j+1;
            reg.nRegistro=rand()%REGMAX;
            //printf("proceso %d: valor de rand: %d\n", i, reg.nRegistro );
            int error = mi_write(final, &reg, reg.nRegistro*sizeof(struct REGISTRO), sizeof(struct REGISTRO));
            if (error  < 0){
              printf("Proceso %d: Error de escritura\n", i);
            }
            usleep(50000);
          }
          printf("Proceso %d: Completadas las %d escrituras en %s\n", i, NUM_ESCRITURAS, final);  
          bumount();
          exit(0);
        }
        usleep(200000);
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

