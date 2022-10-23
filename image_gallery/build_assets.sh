index=0
for image in ./assets/original/*; do
    image_gb="./assets/gb/image_$index.png"
    convert "$image" -resize 160x144! +dither -colors 4 $image_gb
    png2asset $image_gb -map -c "src/image_$index.c"
    index=$(( index + 1 ))
done 