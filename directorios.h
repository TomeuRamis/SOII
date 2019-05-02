#include "ficheros.h"

struct entrada {
char nombre[60];  //En el SF ext2 la longitud del nombre es 255
unsigned int ninodo;
};

int extraer_camino(const char *camino, char *inicial, char *final);
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos);

//Nivel 9
int mi_creat(const char *camino, unsigned char permisos);
int mi_dir(const char *camino, char *buffer);