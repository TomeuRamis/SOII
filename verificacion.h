#include "simulacion.h"
#define TAM_BIG_BUF 10000
struct INFORMACION {
    int pid; 
    unsigned int nEscrituras; // validadas 
    struct REGISTRO PrimeraEscritura; 
    struct REGISTRO UltimaEscritura;
    struct REGISTRO MenorPosicion;
    struct REGISTRO MayorPosicion;
};
