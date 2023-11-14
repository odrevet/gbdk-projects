# sprite
png2asset "./assets/mario.png" -c "src/graphics/mario.c"  -sw 16 -sh 16 
png2asset "./assets/enemies.png" -c "src/graphics/enemies.c"  -sw 16 -sh 16 

# background
png2asset "./assets/text.png" -c "src/graphics/text.c" -keep_palette_order -map -tiles_only -noflip # noflip because W is a reversed M
png2asset "./assets/level_1_1.png" -c "res/level_1_1.c" -transposed -noflip -map -bin -tile_origin 36
gbcompress --alg=rle res/level_1_1_map.bin res/level_1_1_map.bin.rle
rm res/level_1_1_map.bin

# force compilation
touch src/main.c
