# sprite
png2asset "assets/mario.png" -o "src/graphics/mario.c" -sw 16 -sh 16
png2asset "assets/enemies.png" -o "src/graphics/enemies.c" -sw 16 -sh 16

text_tiles_count=40
common_tiles_count=40
birabuto_tiles_count=60

# background
png2asset "assets/text.png" -o "src/graphics/text" -keep_palette_order -map -tiles_only -noflip
png2asset "assets/tilesets/common.png" -o "res/common" -map -bin -tiles_only -noflip -keep_duplicate_tiles -tile_origin $text_tiles_count
png2asset "assets/tilesets/birabuto.png" -o "res/birabuto" -map -bin -tiles_only -noflip  -keep_duplicate_tiles -tile_origin $(($text_tiles_count + $common_tiles_count))
png2asset "assets/level_1_1.png" \
          -o "res/level_1_1" \
          -source_tileset "assets/tilesets/common.png" \
          -source_tileset "assets/tilesets/birabuto.png" \
          -map -bin -transposed -noflip -keep_duplicate_tiles \
          -tile_origin $text_tiles_count
gbcompress --alg=rle res/level_1_1_map.bin res/level_1_1_map.bin.rle
rm res/level_1_1_map.bin

# force compilation
touch src/main.c
