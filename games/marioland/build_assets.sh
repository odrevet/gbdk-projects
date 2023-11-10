png2asset "./assets/font.png" -c "src/graphics/font.c" -spr8x8 -tiles_only
png2asset "./assets/mario.png" -c "src/graphics/mario.c"  -sw 16 -sh 16 
png2asset "./assets/enemies.png" -c "src/graphics/enemies.c"  -sw 16 -sh 16 

png2asset "./assets/level_1_1.png" -c "res/level_1_1.c" -transposed -noflip -map -bin 
gbcompress --alg=rle res/level_1_1_map.bin res/level_1_1_map.bin.rle
rm res/level_1_1_map.bin

png2asset "./assets/level_1_2.png" -c "res/level_1_2.c" -transposed -noflip -map -bin 
gbcompress --alg=rle res/level_1_2_map.bin res/level_1_2_map.bin.rle
rm res/level_1_2_map.bin

png2asset "./assets/level_1_3.png" -c "res/level_1_3.c" -transposed -noflip -map -bin 
gbcompress --alg=rle res/level_1_3_map.bin res/level_1_3_map.bin.rle
rm res/level_1_3_map.bin

# force compilation
touch src/main.c
