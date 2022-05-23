#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <vector>

using namespace std;
#define PI 3.14159265358979
int QuantizationMatrix[64] = {3, 5,	7, 9, 11, 13, 15, 17, 5, 7,	9, 11, 13, 15, 17, 19, 7, 9, 11, 13, 15, 17, 19, 21, 9, 11,	13,	15,	17,	19,	21,	23, 11,	13,	15,	17,	19,	21,	23,	25, 13,	15,	17,	19,	21,	23,	25,	27, 15,	17,	19,	21,	23,	25,	27,	29, 17,	19,	21,	23,	25,	27,	29,	31};

double dctColumn[256][256];

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "Arguments prompt: viewSynthesis.exe <img_path> <apply_quantization>" << endl;
		return 0;
	}
	string imgPath = argv[1];
	bool need_quantization = stoi(argv[2]);

	//! read input image
	Bitmap s_img(imgPath.c_str());
	int rows = s_img.getHeight(), cols = s_img.getWidth();
	cout << "Apply DCT on image ("<<rows<<", "<<cols<< ")." << endl;
	
	//! preprocess by shifting pixel values by 128
	//TODO
	int x, y;
	unsigned char temp;
	int temp2;

	//as it is assume that the input image resolution is fixed to 256 by Ta's code in default in the following section, so I assume the declare of temp 2D array is also fixed to 256

	int tempPixel[256][256];
	
	for(x=0; x<rows; x++){
		for(y=0; y<cols;y++){
			s_img.getPixel(x,y, temp);
			tempPixel[x][y]=temp-128;
		}
	}
	
	//! 2D DCT for every 8x8 block (assume that the input image resolution is fixed to 256)	
	double coeffArray[256][256]={0};
	
	int blockRow = rows / 8, blockCol = cols / 8;
	int u, v;
	int sum_x, sum_y;
	double result=0;


	for (int i = 0; i < blockRow; i++)
	{
		for (int j = 0; j < blockCol; j++)
		{
			//for reach blocks of 8x8 pixel
			//total: 32 blocks in a row

			int xpos = j*8, ypos = i*8;
			//! apply DCT on block_ij (basic requirement)
			//TODO
			//for each block
			if(need_quantization==0){
				for(v=0;v<8;v++){
					for(u=0;u<8;u++){
						//Apply 1D DCT to row v of Shifted pixels
						for(sum_x=0;sum_x<8;sum_x++){
							result += tempPixel[xpos+sum_x][ypos+v]*cos(u*PI*(2*sum_x+1)/16);
						}
						if(u==0){
							dctColumn[xpos+u][ypos+v]=result*(1/sqrt(2))/2;
						}else{
							dctColumn[xpos+u][ypos+v]=result/2;
						}
						result = 0;
					}
				}

				for(v=0;v<8;v++){
					for(u=0;u<8;u++){
						//Apply 1D DCT to column u of Rp
						for(sum_y=0;sum_y<8;sum_y++){
							result += dctColumn[xpos+u][ypos+sum_y]*cos(v*PI*(2*sum_y+1)/16);
						}
						if(v==0){
							coeffArray[xpos+u][ypos+v]=result*(1/sqrt(2))/2;
						}else{
							coeffArray[xpos+u][ypos+v]=result/2;
						}
						result=0;
					}
				}		
			}else if(need_quantization==1){
				//! quantize the frequency coefficient of this block (enhanced part)
				//TODO

				for(v=0;v<8;v++){
					for(u=0;u<8;u++){
						//Apply 1D DCT to row v of Shifted pixels
						for(sum_x=0;sum_x<8;sum_x++){
							result += tempPixel[xpos+sum_x][ypos+v]*cos(u*PI*(2*sum_x+1)/16);
						}
						if(u==0){
							dctColumn[xpos+u][ypos+v]=result*(1/sqrt(2))/2;
						}else{
							dctColumn[xpos+u][ypos+v]=result/2;
						}
						result = 0;
					}
				}

				for(v=0;v<8;v++){
					for(u=0;u<8;u++){
						//Apply 1D DCT to column u of Rp
						for(sum_y=0;sum_y<8;sum_y++){
							result += dctColumn[xpos+u][ypos+sum_y]*cos(v*PI*(2*sum_y+1)/16);
						}
						if(v==0){
							coeffArray[xpos+u][ypos+v]=result*(1/sqrt(2))/2;
						}else{
							coeffArray[xpos+u][ypos+v]=result/2;
						}
						result=0;
					}
				}

				for(v=0;v<8;v++){
					for(u=0;u<8;u++){
						//quantization
						coeffArray[xpos+u][ypos+v]=coeffArray[xpos+u][ypos+v]/QuantizationMatrix[u+8*v];
						coeffArray[xpos+u][ypos+v]= round(coeffArray[xpos+u][ypos+v]);
					}
				}
			}
		}
	}

	/* see result in command line
	for (int kx = 0; kx < 256; kx++)
	{
		for (int ky = 0; ky < 256; ky++)
		{
			printf("%f ", coeffArray[ky][kx]);
		}
		printf("\n");
	}
	*/
	
	//! output the computed coefficient array
	FILE *fp = fopen("coeffs.txt", "w");
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			fprintf(fp, "%3.3lf ", coeffArray[c][r]);
		}
		fprintf(fp, "\n");
	}
	cout << "Result saved!" << endl;
	return 0;
}