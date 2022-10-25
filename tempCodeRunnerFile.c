#include <math.h>
#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./headers/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./headers/stb_image_write.h"

/**
 * Create a new 1-dimensional array with the given size
 * @param[in] _size the size of the array
 * @param[out] _ empty 1-dimensional array filled with 0
 */
unsigned char *uc_arrayNew_1d(int _size)
{
    return (unsigned char *)calloc(_size, sizeof(unsigned char));
}

/**
 * Delete a quarter of the image
 * @param[in] image the input image
 * @param[in] width the width of the image
 * @param[in] height the height of the image
 * @param[in] channel the channel of the image
 */
unsigned char *mask_image(unsigned char *background, unsigned char *foreground, int width, int height, int channel)
{
    unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
    unsigned char *temp_array_2 = uc_arrayNew_1d(width * height * channel);
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
        
                if(abs(foreground[i * width * channel + j * channel + 1] - background[i * width * channel + j * channel + 1]) <= 79 &&
                   abs(foreground[i * width * channel + j * channel + 2] - background[i * width * channel + j * channel + 2]) <= 79 &&
                   abs(foreground[i * width * channel + j * channel + 3] - background[i * width * channel + j * channel + 3]) <= 79 ){
                    for(int k = 0; k < channel; k++)
                    temp_array[i * width * channel + j * channel + k] = 0;
                    }
                else
                {
                    for (int k = 0; k < channel; k++){
                    temp_array[i * width * channel + j * channel + k] = foreground[i * width * channel + j * channel + k];
                    }
            }
        }
    }
    return temp_array;
}

unsigned char *weather_forecast(unsigned char *background, unsigned char *foreground, unsigned char *weather, int width, int height, int channel){
    unsigned char *woman = mask_image(background, foreground, width, height, channel);
    unsigned char *temp_array = uc_arrayNew_1d(width * height * channel);
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            for(int k = 0; k < channel; k++){
                if(woman[i * width * channel + j * channel + k] == 0){
                    woman[i * width * channel + j * channel + k] = weather[i * width * channel + j * channel + k];
                }
            }
        }
    }
    return woman;
}

int main()
{
    // declare variables
    int width, height, channel;
    char fore_path[] = "./images/foreground.png";
    char back_path[] = "./images1/background.jpg";
    char weat_path[] = "./images1/new_background.jpg";
    char save_path[] = "./images1/New.png";
    

    // read image data
    unsigned char *weat_image = stbi_load(weat_path, &width, &height, &channel, 0);
    unsigned char *fore_image = stbi_load(fore_path, &width, &height, &channel, 0);
    unsigned char *back_image = stbi_load(back_path, &width, &height, &channel, 0);
    if (back_image == NULL || fore_image == NULL || weat_image == NULL)
    {
        printf("\nError in loading the image\n");
        exit(1);
    }
    printf("Width = %d\nHeight = %d\nChannel = %d\n", width, height, channel);

    // fill a quarter of the image with black pixels
    unsigned char *mimage = weather_forecast(back_image, fore_image, weat_image, width, height, channel);
    stbi_write_png(save_path, width, height, channel, mimage, width * channel);
    printf("New image saved to %s\n", save_path);
}