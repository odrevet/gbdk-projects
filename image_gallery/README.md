Display an full screen image

To convert the input image into a 4 colors image, `ImageMagick` must be installed.

* Use the `build_assets.sh` with the image to use as parameter.

An example `image.png` is provided (fun fact: this image is generated unsing Stable Diffusion)

example:

`bash build_assets.sh` will convert images to `assets/gb`, generate image as C array to `src/image.h` and `src/image.c`

* build the rom with `make`


Limited to 4 images at the moment