Display full screen images

To convert the input images into 4 colors images, `ImageMagick` must be installed.

* Put images in `assets/original`

* Use the `build_assets.sh`

example:

`bash build_assets.sh` will convert images to `assets/gb`, generate image as C array to `src`

* build the rom with `make`


All images in the `assets/orignal` directory are packed into the rom. 

When too much images are used, make will fails this the message `error: size of the buffer is too small.`

Around 7/8 images can be packed depending on the images. Using banks could be used to pack more images. 