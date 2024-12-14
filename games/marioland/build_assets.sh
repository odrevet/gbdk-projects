# sprite
png2asset "assets/mario.png" -o "src/graphics/mario.c" -sw 16 -sh 16
png2asset "assets/enemies.png" -o "src/graphics/enemies.c" -sw 16 -sh 16

text_tiles_count=40
common_tiles_count=40
birabuto_tiles_count=60
muda_tiles_count=60

# background
png2asset "assets/text.png" -o "src/graphics/text" -keep_palette_order -map -tiles_only -noflip
png2asset "assets/tilesets/common.png" -o "src/graphics/common" -map -bin -tiles_only -noflip -keep_duplicate_tiles -tile_origin $text_tiles_count
png2asset "assets/tilesets/birabuto.png" -o "src/graphics/birabuto" -map -bin -tiles_only -noflip  -keep_duplicate_tiles -b 255 -tile_origin $(($text_tiles_count + $common_tiles_count))
png2asset "assets/tilesets/muda.png" -o "src/graphics/muda" -map -bin -tiles_only -noflip  -keep_duplicate_tiles -b 255 -tile_origin $(($text_tiles_count + $common_tiles_count))

convert_level_asset() {
    local level_number=$1
    local source_tileset=$2

    png2asset "assets/level_${level_number}.png" \
        -o "src/levels/level_${level_number}.c" \
        -source_tileset "assets/tilesets/common.png" \
        -source_tileset "assets/tilesets/${source_tileset}.png" \
        -map -noflip -keep_duplicate_tiles \
        -tile_origin "$text_tiles_count" \
        -b 255
}

echo "level 1_1"
convert_level_asset 1_1 birabuto

echo "level 1_2"
convert_level_asset 1_2 birabuto

echo "level 1_3"
convert_level_asset 1_3 birabuto

echo "level 2_1"
convert_level_asset 2_1 muda

# force compilation
touch src/main.c
