#include "bloques.h"
#include "semaforo_mutex_posix.h"

static sem_t *mutex;
static unsigned int inside_sc = 0;
void mi_waitSem(){
    if (!inside_sc) {
        waitSem(mutex);
    }
    inside_sc++;
}
void mi_signalSem() {
    inside_sc--;
    if (!inside_sc) {
        signalSem(mutex);
    }
}

static int descriptor = 0;



// Mount the file descriptor
int bmount(const char *camino){
    mutex = initSem();
    descriptor = open(camino, O_RDWR|O_CREAT, 0666);
    if (descriptor == -1){
        return -1;
    }
    return descriptor;
}

// Unmount the file descriptor
int bumount(){ 
    descriptor =  close(descriptor);
    deleteSem();
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

