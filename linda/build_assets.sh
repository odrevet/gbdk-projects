png2asset assets/linda.png -sw 16 -sh 32 -c src/graphics/linda.c
png2asset assets/city.png -sw 8 -sh 8 -spr8x8 -c src/graphics/city.c
convert assets/lindasd.png -colors 8 
png2asset assets/lindagb.png -map -c src/graphics/lindagb.c