#include "directorios.h" //el más avanzado .h

int main(int argc, char **argv){
    
    size_t size = 100;
    char *dir = malloc(size);
    
    /*
    If getcwd works and doesn't return NULL we mount our 
    file descriptor on a new file called "argv[1]".
    */
    if (getcwd(dir, size) != NULL){
        strcat(dir, "/");
        bmount( strcat(dir, argv[1]));
    }

    /*
    Using buf as an array of BLOCKSIZE lenght filled with 0's,
    we write argv[2] times buf on the file.
    */
    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);
    printf("%s \n", argv[2]);
    int nbloques = atoi(argv[2]);
    int ninodos = nbloques/4;

    for (int i = 0; i < nbloques; i++){
        bwrite(i, buf);
    }

    initSB(nbloques, ninodos);
    initMB();
    initAI();

    reservar_inodo('d', 7);

    bumount(); //Unmount file descriptor
}
