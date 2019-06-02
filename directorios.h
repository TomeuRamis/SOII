#include "ficheros.h"

#define MI_EXIT_SUCCESS 1
#define ERROR_EXTRAER_CAMINO -1
#define ERROR_PERMISO_LECTURA -2
#define ERROR_NO_EXSISTE_ENTRADA_CONSULTADA -3
#define ERROR_NO_SE_PUEDE_CREAR_ENTRADA_EN_UN_FICHERO -4
//#define ERROR_PERMISO_LECTURA -5
#define ERROR_NO_EXSISTE_DIRECTORIO_INTERMEDIO -6
#define MI_EXIT_FAILURE -7
#define ERROR_ENTRADA_YA_EXSISTENTE -8


struct entrada {
char nombre[60];  //En el SF ext2 la longitud del nombre es 255
unsigned int ninodo;
};

int extraer_camino(const char *camino, char *inicial, char *final);
int buscar_entrada(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos);
int buscar_entrada2(const char *camino_parcial, unsigned int *p_inodo_dir,
unsigned int *p_inodo, unsigned int *p_entrada, char reservar, unsigned char permisos, int a);
//Nivel 9
int mi_creat(const char *camino, unsigned char permisos);
int mi_dir(const char *camino, char *buffer);
int mi_chmod(const char *camino, unsigned char permisos);
int mi_stat(const char *camino, struct STAT *p_stat);

//nivel 10
int mi_read(const char *camino, void *buf, unsigned int offset, unsigned int nbytes);
int mi_write(const char *camino, const void *buf, unsigned int offset, unsigned int nbytes);

//nivel 11
int mi_link(const char *camino1, const char *camino2);
int mi_unlink(const char *camino);
