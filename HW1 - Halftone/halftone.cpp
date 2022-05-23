/*

CSCI 3280, Introduction to Multimedia Systems
Spring 2020

Assignment 01 Skeleton

halftone.cpp

*/

#include "bmp.h"		//	Simple .bmp library
#include <iostream>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define MAX_SHADES 3

#define SAFE_FREE(p)  { if(p){ free(p);  (p)=NULL;} }

using namespace std;

int main(int argc, char** argv)
{
	//
	//	Your code goes here ....
	//
	//	1. Open BMP file
	//  you can use the following parameters for testing,
	//  remember to modify it based on the user input.

	int imagesize;
	int patchsize;
    const char* input_image;

	if(argc<2)
    {
        //default
        imagesize = 64;
        patchsize = 16;
        input_image="monokuma.bmp";
    }
    else if (argc==2)
    {
        imagesize = 64;
        patchsize = 16;
        input_image = argv[1];
    }
    else if (argc==3)
    {
        input_image = argv[1];
        imagesize = atoi(argv[2]);
        patchsize = 16;
    }
    else if(argc==4)
    {
        //cout<<argv[1]<<endl;
        //cout<< argc<<endl;
        input_image = argv[1];
        imagesize = atoi(argv[2]);
        patchsize = atoi(argv[3]);
    }

    Bitmap image_data(input_image);
	image_data.resize(imagesize);


	//	2. Load image patches
	//

    Bitmap patch_0(".\\patch\\0.bmp");
    Bitmap patch_1(".\\patch\\1.bmp");
    Bitmap patch_2(".\\patch\\2.bmp");

    patch_0.resize(patchsize);
    patch_1.resize(patchsize);
    patch_2.resize(patchsize);

	//
	//	3. Obtain Luminance
	//

    int resized_height = image_data.getHeight();
    int resized_width  = image_data.getWidth();

    Bitmap temp(resized_width,resized_height);

    int x,y;
    unsigned char r,g,b;
    unsigned char gray=0;
    unsigned char quantized;

    for(x=0;x<resized_width;x++)
    {
        for(y=0;y<resized_height;y++)
        {
            image_data.getColor(x,y,r,g,b);
            gray = 0.299*(int)r + 0.587*(int)g + 0.114*(int)b;
            temp.setColor(x,y,gray,gray,gray);
        }
    }

    //temp.save("test.bmp");

	//
	//	4. Quantization
	//

    for(x=0;x<resized_width;x++)
    {
        for(y=0;y<resized_height;y++)
        {
            temp.getColor(x,y,r,g,b);  //r == g == b
            gray = r;
            quantized = floor(((int)gray)/(256/3));  //prevent equal to 3
            //cout<< (int)quantized << endl;
            temp.setColor(x,y,quantized,quantized,quantized);
        }
    }

	//
	//  5. Generate bmp image and parse patches according to quantized image
	//

    int result_width = resized_width*patchsize;
    int result_height = resized_height*patchsize;

    int resized_patch_size = patch_0.getHeight();  //assume all patch size are the same

    //cout<<result_width<<endl;
    //cout<<result_height<<endl;

    Bitmap result(result_width,result_height);

    unsigned char p_r;
    unsigned char p_g;
    unsigned char p_b;
    int p_x, p_y;

    for(x=0;x<resized_width;x++)  //for each "big pixel"
    {
        for(y=0;y<resized_height;y++)
        {
            temp.getColor(x,y,r,g,b);
            //cout<<(int)r<<endl;
            if(int(r)==0)
            {
                for(p_x=0;p_x<resized_patch_size;p_x++)  //for each "small" pixel
                {
                    for(p_y=0;p_y<resized_patch_size;p_y++)
                    {
                        patch_0.getColor(p_x,p_y,p_r,p_g,p_b);
                        result.setColor( (p_x+x*resized_patch_size) , (p_y+y*resized_patch_size) ,p_r,p_g,p_b);
                    }
                }
            }
            else if(int(r)==1)
            {
                for(p_x=0;p_x<resized_patch_size;p_x++)  //for each "small" pixel
                {
                    for(p_y=0;p_y<resized_patch_size;p_y++)
                    {
                        patch_1.getColor(p_x,p_y,p_r,p_g,p_b);
                        result.setColor( (p_x+x*resized_patch_size) , (p_y+y*resized_patch_size) ,p_r,p_g,p_b);
                    }
                }
            }
            else
            {
                for(p_x=0;p_x<resized_patch_size;p_x++)  //for each "small" pixel
                {
                    for(p_y=0;p_y<resized_patch_size;p_y++)
                    {
                        patch_2.getColor(p_x,p_y,p_r,p_g,p_b);
                        result.setColor( (p_x+x*resized_patch_size) , (p_y+y*resized_patch_size) ,p_r,p_g,p_b);
                    }
                }
            }
        }
    }

    result.save("result.bmp");

    //  free memory
    image_data.~Bitmap();
    temp.~Bitmap();
    patch_0.~Bitmap();
    patch_1.~Bitmap();
    patch_2.~Bitmap();
    result.~Bitmap();

	return 0;
}
