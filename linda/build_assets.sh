png2asset assets/linda.png -sw 16 -sh 32 -c src/graphics/linda.c
png2asset assets/city.png -sw 8 -sh 8 -spr8x8 -c src/graphics/city.c
convert assets/title_background_original.png -resize 160x144! +dither -colors 4 assets/title_background.png
png2asset assets/title_background.png -map -c src/graphics/lindagb.c