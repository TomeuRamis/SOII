#!/bin/bash

################################################################################
./mi_mkfs disco 100000
################################################################################
./mi_mkdir disco 6 /dir1/
./mi_mkdir disco 6 /dir1/dir11/
./mi_touch disco 6 /dir1/dir11/fic1 #o mi_mkdir
./mi_escribir disco /dir1/dir11/fic1 hellooooooo 0
################################################################################
./mi_mkdir disco 6 /dir2/
./mi_mkdir disco 6 /dir2/dir21/
################################################################################
./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2
################################################################################
./mi_cat disco /dir2/dir21/fic2 
################################################################################
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4  #camino1 ha de 
./mi_touch disco 6 /dir1/dir11/fic3 #o mi_mkdir
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic5
./mi_stat disco /dir1/dir11/fic3

./mi_rm disco /dir2/dir21/ #o mi_rmdir

