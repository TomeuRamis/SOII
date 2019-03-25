#include "bloques.h"

static int descriptor = 0;

// Mount the file descriptor
int bmount(const char *camino){
    descriptor = open(camino, O_RDWR|O_CREAT, 0666);
    if (descriptor == -1){
        return -1;
    }
    return descriptor;
}

// Unmount the file descriptor
int bumount(){
    descriptor =  close(descriptor);
    if (descriptor ==-1){
        return -1;
    }
    return 0;
}

//Write the content of buf on the block number nbloques of the file
int bwrite(unsigned int nbloque, const void *buf){
    size_t nbytes;
    off_t desplazamiento = nbloque*BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);
    nbytes = write(descriptor, buf, BLOCKSIZE);
    if (nbytes < 0){
        return -1;
    }
    return nbytes;
}

// Read the block at the position 'nbloques' and fill buf with the content 
int bread(unsigned int nbloque, void *buf){
    size_t nbytes;
    off_t desplazamiento = nbloque*BLOCKSIZE;
    lseek(descriptor, desplazamiento, SEEK_SET);
    nbytes = read(descriptor, buf, BLOCKSIZE);
    if (nbytes < 0){
        return -1;
    }
    return nbytes;
}

