#include "ficheros_basico.h"

int tamMB(unsigned int nbloques){
    int tamMB =((nbloques/8)/BLOCKSIZE);
    if((tamMB % BLOCKSIZE)!=0){
        tamMB++;
    }
    return tamMB;
}

int tamAI(unsigned int ninodos){
    int tamAI = ((ninodos*INODOSIZE)/BLOCKSIZE);
    if((BLOCKSIZE % INODOSIZE)!=0){ //////////////////////////////PORQUE POFABO ???????????? PROBABLEMENTE SEA TAMAI % BLOCKSIZE
        tamAI++;
    }
    
    return tamAI;
}

int initSB(unsigned int nbloques, unsigned int ninodos){
    int posSB = 0;
    int tamSB = 1;
    SB.posPrimerBloqueMB = posSB + tamSB; //posSB = 0, tamSB = 1
    SB.posUltimoBloqueMB = SB.posPrimerBloqueMB+tamMB(nbloques)-1;
    SB.posPrimerBloqueAI = SB.posUltimoBloqueMB+1;
    SB.posUltimoBloqueAI = SB.posPrimerBloqueAI+tamAI(ninodos)-1;
    SB.posPrimerBloqueDatos = SB.posUltimoBloqueAI+1;
    SB.posUltimoBloqueDatos = nbloques-1;
    SB.posInodoRaiz = 0 ;
    SB.posPrimerInodoLibre = 0;
    SB.cantBloquesLibres = nbloques;
    SB.cantInodosLibres = ninodos;
    SB.totBloques = nbloques;
    SB.totInodos = ninodos;
    SB.cantBloquesLibres = SB.cantBloquesLibres - tamSB;
    bwrite(posSB, &SB);
    return 0;
}

int initMB(){  
    unsigned char buf[BLOCKSIZE];
    memset(buf, 0, BLOCKSIZE);
    for (int i = SB.posPrimerBloqueMB; i <= SB.posUltimoBloqueMB ; i++){
        bwrite(i, buf);
    }
    for(int i = 0; i< SB.posUltimoBloqueAI;i++){
        escribir_bit(i,1);
    }
    SB.cantBloquesLibres = SB.cantBloquesLibres -(SB.posUltimoBloqueMB - SB.posPrimerBloqueMB) - 1;
    bwrite(0, &SB);
    return 0;
}

int initAI(){
    int contInodos = SB.posPrimerInodoLibre +1;
    for(int i= SB.posPrimerBloqueAI; i <= SB.posUltimoBloqueAI;i++){
        for(int j=0;j<(BLOCKSIZE/INODOSIZE);j++){
            inodos[j].tipo='l';
            if(contInodos < SB.totInodos){
                inodos[j].punterosDirectos[0]=contInodos;
                contInodos++;
            }else{
                inodos[j].punterosDirectos[0]=UINT_MAX;
            }
        }
        bwrite(i,inodos);    
    }
    SB.cantBloquesLibres = SB.cantBloquesLibres - (SB.posUltimoBloqueAI - SB.posPrimerBloqueAI) - 1;
    return 0;
}

int escribir_bit(unsigned int nbloque, unsigned int bit){
    unsigned char mascara = 128; //10000000

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;
    int nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
    unsigned char bufferMB[BLOCKSIZE];
    bread(nbloqueabs,bufferMB);   //si falla mirar aqui
    posbyte = posbyte % BLOCKSIZE;
    mascara >>= posbit;

    if(bit ==1){
        bufferMB[posbyte] |= mascara;
    }else{
        bufferMB[posbyte] &= ~mascara;
    }
    bwrite(nbloqueabs,bufferMB);
    return 0;
}

unsigned char leer_bit (unsigned int nbloque){ //revisar
    unsigned char mascara = 128; //10000000

    int posbyte = nbloque / 8;
    int posbit = nbloque % 8;
    int nbloqueMB = posbyte / BLOCKSIZE;
    int nbloqueabs = nbloqueMB + SB.posPrimerBloqueMB;
    unsigned char bufferMB[BLOCKSIZE];
    bread(nbloqueabs,bufferMB);   //si falla mirar aqui
    posbyte = posbyte % BLOCKSIZE;
    mascara >>= posbit;
    mascara &=bufferMB[posbyte];
    mascara >>= (7-posbit);
    return mascara;
}

int reservar_bloque(){

    if(SB.cantBloquesLibres > 0){
        int encontrado = 0; // 0 -> False; 1 -> True 
        int nbloque = 0;         
        unsigned char bufferMB[BLOCKSIZE];
        unsigned char bufferAux[BLOCKSIZE];   
        memset(bufferAux, 255, BLOCKSIZE);
        int posBloqueMB = SB.posPrimerBloqueMB;
        while(posBloqueMB < SB.posUltimoBloqueMB && encontrado == 0){
            bread(posBloqueMB, bufferMB);
            if(memcmp(bufferMB, bufferAux, BLOCKSIZE) != 0){
                for(int posbyte = 0; posbyte < BLOCKSIZE && encontrado == 0; posbyte++){
                    if(bufferMB[posbyte] != bufferAux[posbyte]){
                        unsigned char mascara = 128; // 10000000
                        int posbit = 0;
                        while (bufferMB[posbyte] & mascara) {  
                            posbit++;
                            bufferMB[posbyte] <<= 1; // desplaz. de bits a la izqda
                        }
                        encontrado = 1;
                        nbloque = ((posBloqueMB - SB.posPrimerBloqueMB) * BLOCKSIZE+ posbyte) * 8 + posbit;

                    }
                }
            }
            posBloqueMB++;
        }
        escribir_bit(nbloque, 1);
        SB.cantBloquesLibres--;     
        unsigned char buf[BLOCKSIZE];
        memset(buf, 0, BLOCKSIZE);
        bwrite(nbloque, buf);
        bwrite(0, &SB);
        return nbloque;
    } else{
        printf("No quedan bloques libres.");
        return -1;
    }
    
}

int liberar_bloque(unsigned int nbloque){ //revisar
    escribir_bit(nbloque,0);
    SB.cantBloquesLibres++;
    bwrite(0, &SB);
    return nbloque;
}

int escribir_inodo(unsigned int ninodo, struct inodo inodo){ 

    int posBloqueInodo =  SB.posPrimerBloqueAI + (ninodo/8); 
    struct inodo ino2[BLOCKSIZE/INODOSIZE];
    bread(posBloqueInodo,ino2);
    int pos = ninodo % (BLOCKSIZE/INODOSIZE);
    ino2[pos]= inodo;
    bwrite(posBloqueInodo,ino2); 
    
    return 0;
}

int leer_inodo(unsigned int ninodo, struct inodo *inodo){
    int posBloqueInodo = SB.posPrimerBloqueAI + (ninodo/8); 
    struct inodo inodos[BLOCKSIZE/INODOSIZE];
    bread(posBloqueInodo,inodos);
    int pos = ninodo % (BLOCKSIZE/INODOSIZE);
    *inodo = inodos[pos]; //esto está regulero
    if (inodo != NULL) {
        return 0;
    }
    return -1;
}

int reservar_inodo(char tipo, unsigned char permisos){
    if(SB.cantInodosLibres > 0){
        int posInodoReservado = SB.posPrimerInodoLibre;
        struct inodo inodo;
        //SB.posPrimerInodoLibre++;                          /////////////////CUESTIONABLE DECISION/////////////
        inodo.tipo = tipo;
        inodo.permisos = permisos;
        inodo.nlinks = 1;
        inodo.tamEnBytesLog = 0;
        inodo.atime = time(NULL);
        inodo.ctime = time(NULL);
        inodo.mtime = time(NULL);
        inodo.numBloquesOcupados = 0;
        for (int i = 0; i <= DIRECTOS; i++){
            inodo.punterosDirectos[i] = 0;
        }
        for(int i = 0; i <= 3; i++){
            inodo.punterosIndirectos[i] = 0;
        }
        //inodo.punterosDirectos = 0; ERRORIZACIÓN REVISAr!
        //inodo.punterosIndirectos[0] = 0;
        escribir_inodo(posInodoReservado, inodo);
        SB.posPrimerInodoLibre++;
        SB.cantInodosLibres--;
        bwrite(0, &SB);
        return posInodoReservado;
    } else {
        printf("No quedan inodos libres");
        return -1;
    }
}    

int obtener_nrangoBL(struct inodo inodo, unsigned int nblogico, unsigned int *ptr){
    int nrangoBL=-1;
    printf("nblogico en obtener nrangoBL: %d\n", nblogico);
    if(nblogico<DIRECTOS){ /////////////////////// peligro asterisco
        *ptr=inodo.punterosDirectos[nblogico];
        printf("Valor ptr: %d\n", *ptr);
        nrangoBL = 0;
        printf("Inodo.PunterosDirectos[%d]\n", nblogico);
    }else if(nblogico<INDIRECTOS0){
        *ptr=inodo.punterosIndirectos[0];
        nrangoBL = 1;    
        //printf("Inodo.PunterosIndirectos[0]\n");
    }else if(nblogico<INDIRECTOS1){
        *ptr=inodo.punterosIndirectos[1];
        nrangoBL = 2;
        //printf("Inodo.PunterosIndirectos[1]\n");
    }else if(nblogico<INDIRECTOS2){
        *ptr=inodo.punterosIndirectos[2];
        //printf("Inodo.PunterosIndirectos[2]\n");
        nrangoBL = 3;
    }else{
        *ptr=0;
        printf("Bloque lógico fera de rango");
    }
    return nrangoBL;
}
    
int obtener_indice(int nblogico, int nivel_punteros){
    if(nblogico<DIRECTOS){
        return nblogico;
    }else if(nblogico<INDIRECTOS0){
        return (nblogico-DIRECTOS);
    }else if(nblogico<INDIRECTOS1){
        if(nivel_punteros==2){
            return((nblogico-INDIRECTOS0)/NPUNTEROS);
        }else if(nivel_punteros==1){
            return ((nblogico-INDIRECTOS0)%NPUNTEROS);
        }else{
            printf("Nivel de puntero incorrecto");
        }
    }else if(nblogico<INDIRECTOS2){
        if(nivel_punteros==3){
            return (nblogico-INDIRECTOS1)/(NPUNTEROS*NPUNTEROS);
        }else if(nivel_punteros==2){
            return ((nblogico-INDIRECTOS1)%(NPUNTEROS*NPUNTEROS))/NPUNTEROS;
        }else if(nivel_punteros==1){
            return ((nblogico-INDIRECTOS1)%(NPUNTEROS*NPUNTEROS))%NPUNTEROS;
        }
    }
    return -1;
}

int traducir_bloque_inodo(unsigned int ninodo, unsigned int nblogico, char reservar){
    struct inodo inodo;
    unsigned int ptr, ptr_ant, salvar_inodo,nRangoBL,nivel_punteros,indice;
    int buffer[NPUNTEROS];
    leer_inodo(ninodo,&inodo);
    ptr=0;
    ptr_ant=0;
    salvar_inodo=0;
    printf("NBlogico en traducir_bloque_inodo %d\n", nblogico);
    nRangoBL=obtener_nrangoBL(inodo, nblogico, &ptr);
    nivel_punteros=nRangoBL;
    while(nivel_punteros>0){                    //iterar para cada nivel de indirectos
        //printf("Nivel de punteros: %d -> ",nivel_punteros);
        if(ptr==0){                             //no cuelgan bloques de puntero
            if(reservar==0){
                printf("Error de lectura del bloque inexistente");
                return -1;
            }else{                               //reservar bloques punteros y crear enlaces desde inodo hasta datos
                salvar_inodo=1;
                ptr=reservar_bloque();
                inodo.numBloquesOcupados++;
                inodo.ctime=time(NULL);
                if(nivel_punteros==nRangoBL){   //el bloque cuelga directamente del inodo
                    inodo.punterosIndirectos[nRangoBL-1]=ptr;
                    //printf("bloque fisico reservado: %d, ",inodo.punterosIndirectos[nRangoBL-1]);
                } else {                        //el bloque cuelga de otro bloque de punteros
                    buffer[indice]=ptr;
                    bwrite(ptr_ant,buffer);
                }
            }
        }
        //printf("Punteros nivel%d [%d] = %d (BF %d para punteros_nivel %d)\n", nivel_punteros, indice, ptr, ptr, nivel_punteros);
        bread(ptr,buffer);
        indice=obtener_indice(nblogico,nivel_punteros);
        
        ptr_ant=ptr;
        ptr=buffer[indice];
        nivel_punteros--;
    }                                           //al salir de este bucle ya estamos al nivel de datos
    //printf("valor de ptr: %d\n", ptr);
    if(ptr==0){                                 //no existe bloque de datos
        if(reservar==0){                        //error lectura ∄ bloque  
            printf("Error de lectura del bloque.");
            return -2;
        }else{
            salvar_inodo=1;
            ptr=reservar_bloque();              //de datos
            inodo.numBloquesOcupados++;
            inodo.ctime=time(NULL);
            if(nRangoBL==0){
                inodo.punterosDirectos[nblogico]=ptr;
                //printf("inodo.PunterosDirectos[%d] = %d", inodo.punterosDirectos[nblogico], nblogico);
            }else{
                buffer[indice]=ptr;
                
                bwrite(ptr_ant,buffer);
            }
        }   
        //printf("Bloque fisico: %d -> para BL %d\n",buffer[indice], nblogico);
    }   
    
    if(salvar_inodo==1){
        escribir_inodo(ninodo,inodo);
    }
    //printf("\n");
    return ptr;
}

int liberar_inodo(unsigned int ninodo){
    struct inodo inodo;
    int liberados = liberar_bloques_inodo(ninodo,0);
    leer_inodo(ninodo,&inodo);
    inodo.numBloquesOcupados = inodo.numBloquesOcupados - liberados;
    inodo.tipo= 'l';
    inodo.punterosDirectos[0] = SB.posPrimerInodoLibre;
    SB.posPrimerInodoLibre = ninodo;
    SB.cantInodosLibres++;
    escribir_inodo(ninodo,inodo);
    bwrite(0,&SB);
    return liberados;
}

int liberar_bloques_inodo(unsigned int ninodo, unsigned int nblogico){
    unsigned char bufAux_punteros[BLOCKSIZE]; //1024 bytes
    memset (bufAux_punteros, 0, BLOCKSIZE);
    struct inodo inodo;
    unsigned int nRangoBL, nivel_punteros, indice, ptr, nblog, ultimoBL;
    int bloques_punteros[3][NPUNTEROS];
    int ptr_nivel[3];
    int indices[3];
    int liberados=0;
    int salvar_inodo =0;
    leer_inodo(ninodo,&inodo);
    if (inodo.tamEnBytesLog==0){
         return 0;
    }
    if (inodo.tamEnBytesLog % BLOCKSIZE == 0){
        ultimoBL = inodo.tamEnBytesLog/BLOCKSIZE -1;
    } else {
         ultimoBL = inodo.tamEnBytesLog/BLOCKSIZE;
    }
    //ultimoBL=INDIRECTOS2-1;
    //printf("Primer bloque logico: 0, último BL: %d\n",ultimoBL);
    ptr = 0;
    for(nblog=nblogico; nblog <= ultimoBL; nblog++){//maybe 
        if(traducir_bloque_inodo(ninodo,nblog,0)!=-1){
            printf("BL: %d\n",nblog);
            nRangoBL = obtener_nrangoBL(inodo,nblog,&ptr);
            if(ptr!=0){         //El bloque logico no está vacio ("optimización")
                if (nRangoBL<0){
                    fprintf(stderr, "He petao");
                    return -1;
                }
                nivel_punteros=nRangoBL;          
                while(ptr>0 && nivel_punteros>0){
                    //printf("suh\n");
                    bread(ptr, bloques_punteros[nivel_punteros-1]);
                    indice= obtener_indice(nblog, nivel_punteros);
                    //es poible que aqui se pueda hacer algo
                    // if(nivel_punteros==0){
                    //                 nblog=nblog+255-indice;
                    //             }else if(nivel_punteros==1){
                    //                 nblog=nblog+256*(255-indice);
                    //             }else if(nivel_punteros==2){
                    //                nblog=nblog+256*256*(255-indice);
                    //             }
                    //----------------------------------
                    ptr_nivel[nivel_punteros-1]=ptr;
                    indices[nivel_punteros-1]=indice;
                    ptr=bloques_punteros[nivel_punteros-1][indice];                   
                    nivel_punteros--;
                }
                if (ptr>0){
                    liberar_bloque(ptr);
                    liberados++;
                    //printf("Liberado BF %d de datos correspondiente al BL %d\n",ptr,nblogico);
                    if(nRangoBL == 0) {
                        inodo.punterosDirectos[nblog]=0;
                        salvar_inodo=1;
                    } else {
                        while (nivel_punteros<nRangoBL){
                            //printf("Nivel punteros: %d\n",nivel_punteros);
                            //printf("nRangoBL: %d\n",nRangoBL);
                            indice=indices[nivel_punteros];
                            bloques_punteros[nivel_punteros][indice]=0;
                            ptr=ptr_nivel[nivel_punteros]; 
                            if (memcmp (bloques_punteros[nivel_punteros], bufAux_punteros, BLOCKSIZE)==0){
                                liberar_bloque(ptr);
                                //printf("BL antes: %d\n",nblog);
                                if(nivel_punteros==0){
                                    nblog=nblog+255-indice;
                                }else if(nivel_punteros==1){
                                    nblog=nblog+256*(255-indice);
                                }else if(nivel_punteros==2){
                                    nblog=nblog+256*256*(255-indice);
                                } 
                                //printf("BL despues: %d\n",nblog);
                                //printf("liberado BF %d de punteros de nivel %d correspondiente al BL %d\n",ptr,nivel_punteros,nblogico);
                                liberados++;
                                nivel_punteros++;
                                
                                if(nivel_punteros==nRangoBL){
                                    inodo.punterosIndirectos[nRangoBL-1]=0;
                                    salvar_inodo=1;
                                }
                            } else {
                                bwrite(ptr,bloques_punteros[nivel_punteros]);
                                nivel_punteros=nRangoBL;
                            }  
                        }
                    }
                    //Añadir optimizacion aqui por la buena
                }
            }
        }
    }
    fprintf(stdout, "Numero de bloques liberados: %d\n", liberados);
    if (salvar_inodo==1){
        escribir_inodo(ninodo,inodo);
    }
    return liberados;
}