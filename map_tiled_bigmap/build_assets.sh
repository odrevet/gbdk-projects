png2asset "./assets/font.png" -c "src/graphics/font.c" -spr8x8 -tiles_only
png2asset "./assets/cursor.png" -c "src/graphics/cursor.c" -spr8x8 -tiles_only
png2asset "./assets/World1Tileset.png" -c "src/graphics/World1Tileset.c" -spr8x8 -tiles_only -keep_duplicate_tiles -map
node ../../tiled-gameboy-tool/build/index.js --output-dir src/maps --tiled assets/World1Area1.tmx --bank AUTOBANKED -export-solid-map  --export-objects
sed -i '1d' src/maps/world1area1.c
node ../../tiled-gameboy-tool/build/index.js --output-dir src/maps --tiled assets/World1Area2.tmx --bank AUTOBANKED -export-solid-map  --export-objects
sed -i '1d' src/maps/world1area2.c
