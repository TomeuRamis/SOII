#include "simulacion.h"

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
        char *camino= "/simul_";
        struct tm *ts;
        time_t time = NULL;
        ts = localtime(time);
        strftime(time, sizeof(time_t), "%a %Y-%m-%d %H:%M:%S");
        strcat(camino,(char *)time);
        //mi_creat(camino,6);
        printf("Camino: %ld",time);
        bumount();
    }else{
        printf("Sintaxis: ./simulacion <disco>");
    }
    return 0;
}

