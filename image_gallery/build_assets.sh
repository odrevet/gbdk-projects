# convert image and generate C source files
nb_images=0
for image in ./assets/original/*; do
    image_gb="./assets/gb/image_$nb_images.png"
    convert "$image" -resize 160x144! +dither -colors 4 $image_gb
    png2asset $image_gb -map -c "src/image_$nb_images.c"
    nb_images=$(( nb_images + 1 ))
done 

# generate image_list.h
# include guards
image_list_h="#ifndef IMAGE_LIST\n#define IMAGE_LIST\n\n#define NB_IMAGES $nb_images\n\n"
for ((i=0; i<$nb_images; i++))
do
   image_list_h+="#include \"image_$i.h\"\n"
done

image_list_h+="\n"

# TILE_COUNT array
image_list_h+="const uint8_t TILE_COUNT[] = {"
for ((i=0; i<$nb_images; i++))
do
   image_list_h+="image_${i}_TILE_COUNT"
   if [ "$i" -ne $(( nb_images - 1)) ] 
   then
    image_list_h+=", "
   fi
done
image_list_h+="};\n"

# TILES array
image_list_h+="const uint8_t *TILES[] = {"
for ((i=0; i<$nb_images; i++))
do
   image_list_h+="image_${i}_tiles"
   if [ "$i" -ne $(( nb_images - 1)) ] 
   then
    image_list_h+=", "
   fi
done
image_list_h+="};\n"

# MAP array
image_list_h+="const unsigned char *MAP[] = {"
for ((i=0; i<$nb_images; i++))
do
   image_list_h+="image_${i}_map"
   if [ "$i" -ne $(( nb_images - 1)) ] 
   then
    image_list_h+=", "
   fi
done
image_list_h+="};\n"

image_list_h+="\n"

# end include guard
image_list_h+="#endif"

# write to file
echo -e  $image_list_h > src/image_list.h