#include "directorios.h"
#include <sys/wait.h>
#include <signal.h>
//#include <sys/timeb.h>

#define REGMAX 500000
#define NUMPROCESOS 100
#define NUM_ESCRITURAS 50

struct REGISTRO {
    time_t fecha; //fecha de la escritura en formato epoch 
    //struct timeb fecha;
    pid_t pid; //PID del proceso que lo ha creado
    int nEscritura; //Entero con el número de escritura (de 1 a 50)
    int nRegistro; //Entero con el número del registro dentro del fichero (de 0 a REGMAX-1) 
};


