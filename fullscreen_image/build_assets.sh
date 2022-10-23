image=$1
convert "$image" -resize 160x144! +dither -colors 4 "./assets/image_gb.png"
png2asset "./assets/image_gb.png" -map -c "src/image.c"