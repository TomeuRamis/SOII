#!/bin/bash


################################################################################
./mi_mkfs disco 100000
################################################################################
./mi_mkdir disco 6 /dir1/
./mi_mkdir disco 6 /dir1/dir11/
./mi_mkdir disco 6 /dir1/dir11/fic1 #o mi_mkdir
./mi_escribir disco /dir1/dir11/fic1 hellooooooo 0
################################################################################
./mi_mkdir disco 6 /dir2/
./mi_mkdir disco 6 /dir2/dir21/
################################################################################
./mi_link disco /dir1/dir11/fic1 /dir2/dir21/fic2
################################################################################
./mi_cat disco /dir2/dir21/fic2 #ha de mostrar mismo contenido que /dir1/dir11/fic1

./mi_stat disco /dir1/dir11/fic1

./mi_stat disco /dir2/dir21/fic2
################################################################################
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4  #camino1 ha de existir
./mi_mkdir disco 6 /dir1/dir11/fic3 #o mi_mkdir
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic4
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic5
./mi_stat disco /dir1/dir11/fic3
################################################################################
./mi_link disco /dir1/dir11/fic3 /dir2/dir21/fic2 #camino2 NO ha de existir

./mi_mkdir disco 6 /d1/
# creamos 17 subdirectorios sd0, sd1..., sd16 en d1
for i in $(seq 0 16)
do
./mi_mkdir disco 6 /d1/sd$i/
done

