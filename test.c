#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv){
    char *aux = malloc(10);
    aux="casa";
    while(*aux!=NULL){
        printf("%c",*aux);
    }
}