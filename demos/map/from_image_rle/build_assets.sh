png2asset "./assets/level_large.png" -c "res/level.c" -transposed -noflip -map -bin 
gbcompress --alg=rle res/level_map.bin res/level_map.bin.rle
rm res/level_map.bin

# force compilation
touch src/main.c