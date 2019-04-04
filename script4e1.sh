echo "################################################################################"
echo "$ rm disco"
rm disco
echo "$ ./mi_mkfs disco 100000"
./mi_mkfs disco 100000
echo "################################################################################"
echo "./leer_sf disco"
echo "# Mostramos los campos del superbloque."
./leer_sf disco
echo "################################################################################"
echo "./escribir disco "$(cat texto2.txt)" 1"
echo "# Escribimos el texto contenido en text2.txt en los offsets  0, 5120, 256000, "
echo "# 30720000 y 71680000, de inodos diferentes."
./escribir disco "$(cat texto2.txt)" 1
echo "############################################################################"
echo "./leer_sf disco"
echo "#Mostramos los campos actualizados del superbloque."
./leer_sf disco
echo "################################################################################"
echo "./truncar disco 1 1703"
echo "# Truncamos el inodo 1 por el byte 1.703."
echo "# En el inodo 1 hay 3.751 bytes escritos en el offset 0 (4 bloques de datos:"
echo "# BL 0 ... BL 3). Si truncamos por el byte 1.703 quedarán 2 bloques de datos:"
echo "# BL 0 y BL 1."
./truncar disco 1 1703
echo "################################################################################"
echo "./truncar disco 2 7847"
echo "# Truncamos el inodo 2 por el byte 7.847."
echo "# En el inodo 2 hay 3.751 bytes escritos en el offset 5.120 (tamEnBytesLog=8.871,"
echo "# punterosDirectos[5]... punterosDirectos[8], 4 bloques de datos: BL 5 ... BL 8)."
echo "# Si truncamos por el byte 7.847 quedarán 3 bloques de datos: BL 5 ... BL 7"
./truncar disco 2 7847
echo "################################################################################"
echo "./truncar disco 3 256001"
echo "# Truncamos el inodo 3 por el byte 256.001."
echo "# En el inodo 3 hay 3.751 bytes escritos en el offset 256.000 (tamEnBytesLog="
echo "# 259.751, punterosIndirectos[0], 4 bloques de datos: BL 250 ... BL 253, y 1 "
echo "# bloque punteros de nivel 1)."
echo "# Si truncamos por el byte 256.001 quedará 1 bloque de datos: BL 250 y el bloque"
echo "# de punteros de nivel 1"
./truncar disco 3 256001
echo "################################################################################"
echo "./truncar disco 4 30721024"
echo "# Truncamos el inodo 4 por el byte 30.721.024 (incluido)."
echo "# En el inodo 4 hay 3.751 bytes escritos en el offset 30.720.000 (tamEnBytesLog="
echo "# 30.723.751, punterosIndirectos[1], 4 bloques de datos: BL 30.000 ... BL 30.003, 1 "
echo "# bloque punteros de nivel 1 y 1 bloque de punteros de nivel 2)."
echo "# Si truncamos por el byte 30.721.024 (que es el 1er byte del BL 30.001) quedará"
echo "# 1 bloque de datos: BL 30.000, y los 2 bloques de punteros (nivel 1 y 2)"
./truncar disco 4 30721024
echo "################################################################################"
echo "./truncar disco 5 71682727"
echo "# Truncamos el inodo 5 por el byte 71.682.727."
echo "# En el inodo 5 hay 3.751 bytes escritos en el offset 71.680.000 (tamEnBytesLog="
echo "# 71.683.751, punterosIndirectos[2], 4 bloques de datos: BL 70.000 ... BL 70.003, 1 "
echo "# bloque punteros de nivel 1, 1 bloque de punteros de nivel 2 y un bloque de "
echo "# punteros de nivel 3)."
echo "# Si truncamos por el byte 71.682.727 (que se encuentra en el BL 70.002) quedarán"
echo "# 3 bloque de datos: BL 70.000... BL 70.002 y los 3 bloques de punteros"
./truncar disco 5 71682727
echo "################################################################################"
echo "./leer_sf disco"
echo "# Mostramos de nuevo el SB para comprobar la cantidad de bloques libres del sistema."
./leer_sf disco


