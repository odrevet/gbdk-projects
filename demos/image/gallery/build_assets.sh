# convert image and generate C source files
nb_images=0
for image in ./assets/original/*; do
    image_gb="./assets/gb/image_$nb_images.png"
    convert "$image" -resize 160x144! +dither -colors 4 $image_gb
    
    if png2asset $image_gb -map -b 255 -c "src/image_$nb_images.c" | grep -q "found more than 256 tiles"; then
      echo "ERROR image $nb_images named $image" 
      echo "cannot be converted and will be ignored because it does not contains enougth repeating patterns."
      echo "Try again with -colors 2 or -border 2.5x2.5 or -blur 0.5 options"
      echo ""
      rm src/image_$nb_images.h src/image_$nb_images.c
    else
      nb_images=$(( nb_images + 1 ))
    fi

    
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


# switch bank function
image_list_h+="inline void switch_bank(uint8_t index){\nswitch (index)\n{"
for ((i=0; i<$nb_images; i++))
do
   image_list_h+="case ${i}:\n SWITCH_ROM(BANK(image_$i));\nbreak;\n"
done
image_list_h+="}}\n\n"

# end include guard
image_list_h+="#endif"

# write to file
echo -e  $image_list_h > src/image_list.h