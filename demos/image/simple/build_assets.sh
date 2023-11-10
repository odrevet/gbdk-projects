image=$1
convert "$image" -resize 160x144! +dither -colors 4 "./assets/image_gb.png"
if png2asset "./assets/image_gb.png" -map -c "src/image.c" | grep -q "found more than 256 tiles"; then
    echo "ERROR found more than 256 tiles: This picture cannot be converted because it does not contains enougth repeating patterns. Try again with -colors 2 or -border 2.5x2.5 or -blur 0.5 options"
fi
