png2asset "./assets/World1Tileset.png" -c "src/World1Tileset.c" -spr8x8 -tiles_only -keep_duplicate_tiles -map
node ../../tiled-gameboy-tool/build/index.js --output-dir src --tiled assets/World1Area1.tmx --bank AUTOBANKED
sed -i '1d' src/world1area1.c
