image=$1
convert "$image" -resize 160x144! +dither -colors 4 "./assets/image_gb.png"
if png2asset "./assets/image_gb.png" -map -c "src/image.c" | grep -q "found more than 256 tiles"; then
    echo "ERROR found more than 256 tiles: This picture cannot be converted because it does not contains enough repeating patterns"
fi
