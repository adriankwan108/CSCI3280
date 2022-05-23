#include "bmp.h"		//	Simple .bmp library
#include<iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

#define Baseline 30.0
#define Focal_Length 100
#define Image_Width 35.0
#define Image_Height 35.0
#define Resolution_Row 512
#define Resolution_Col 512
#define View_Grid_Row 9
#define View_Grid_Col 9

struct Point3d
{
	double x;
	double y;
	double z;
	Point3d(double x_, double y_, double z_) :x(x_), y(y_), z(z_) {}
};

struct Point2d
{
	double x;
	double y;
	Point2d(double x_, double y_) :x(x_), y(y_) {}
};


int main(int argc, char** argv)
{
	if (argc != 6)
	{
		cout << "Arguments prompt: viewSynthesis.exe <LF_dir> <X Y Z> <focal_length>" << endl;
		return 0;
	}
	string LFDir = argv[1];
	double Vx = stod(argv[2]), Vy = stod(argv[3]), Vz = stod(argv[4]);
	double targetFocalLen = stod(argv[5]);

	vector<Bitmap> viewImageList;
	//! loading light field views
	for (int i = 0; i < View_Grid_Col*View_Grid_Row; i++)
	{
		char name[128];
		sprintf(name, "/cam%03d.bmp", i+1);
		string filePath = LFDir + name;
		Bitmap view_i(filePath.c_str());
		viewImageList.push_back(view_i);
	}

	Bitmap targetView(Resolution_Col, Resolution_Row);
	cout << "Synthesizing image from viewpoint: (" << Vx << "," << Vy << "," << Vz << ") with focal length: " << targetFocalLen << endl;

	//global coordinate system (virtual camera position)
	Point3d viewPoint(Vx,Vy,Vz);
/*
	double tempX = ((Vx + 120)/30)+1;
	double tempY = ((Vy - 120)/(-30))+1;

	int VpointX1, VpointX2, VpointY1, VpointY2;
	double alpha, theta;
	//find out the four point for interpolation
	if (tempX == (int)tempX)
	{
		//point2D
		VpointX1 = (int)tempX;
		VpointX2 = (int)tempX;
		alpha = 1;
		cout<< "check 1:"<< VpointX1 << endl;
	}else
	{
		VpointX1 = (int)tempX;
		VpointX2 = VpointX1 + 1 ;
		alpha = tempX - (int)tempX;
	}

	if(tempY == (int)tempY)
	{
		VpointY1 = (int)tempY;
		VpointY2 = (int)tempY;
		theta = 1;
		cout<< "check 2:"<< VpointY1 << endl;
	}else
	{
		VpointY1 = (int)tempY;
		VpointY2 = VpointY1 + 1;
		theta = tempY - (int)tempY;
	}
*/
	//cout<< "VpointX1:" << VpointX1 <<endl;
	//cout<< "VpointX2:" << VpointX2 <<endl;

	unsigned char rayTempR1, rayTempG1, rayTempB1;
	unsigned char rayTempR2, rayTempG2, rayTempB2;
	unsigned char rayTempR3, rayTempG3, rayTempB3;
	unsigned char rayTempR4, rayTempG4, rayTempB4;
	unsigned char bindR1, bindG1, bindB1;
	unsigned char bindR2, bindG2, bindB2;

	double ratio;
	double cameraX, cameraY;
	double tempX, tempY;
	int VpointX1, VpointX2, VpointY1, VpointY2;
	double alpha, theta;

	//! resample pixels of the target view one by one
	for (int r = 0; r < Resolution_Row; r++)
	{
		for (int c = 0; c < Resolution_Col; c++)
		{
			Point3d rayRGB(0, 0, 0);
			//! resample the pixel value of this ray: TODO

			
			Point3d destination(0,0,-targetFocalLen);
			Point3d direction(destination.x-viewPoint.x,destination.y-viewPoint.y,destination.z-viewPoint.z);

			ratio = (0-destination.z)/direction.z;
			//cout<<ratio<<endl;
			cameraY = ratio*direction.y + destination.y;
			cameraX = ratio*direction.x + destination.x;

			tempX = ((cameraX + 120)/30)+1;
			tempY = ((cameraY - 120)/(-30))+1;

			if (tempX == (int)tempX)
			{
				//point2D
				VpointX1 = (int)tempX;
				VpointX2 = (int)tempX;
				alpha = 1;
				//cout<< "check 1:"<< VpointX1 << endl;
			}else
			{
				VpointX1 = (int)tempX;
				VpointX2 = VpointX1 + 1 ;
				alpha = tempX - (int)tempX;
			}

			if(tempY == (int)tempY)
			{
				VpointY1 = (int)tempY;
				VpointY2 = (int)tempY;
				theta = 1;
				//cout<< "check 2:"<< VpointY1 << endl;
			}else
			{
				VpointY1 = (int)tempY;
				VpointY2 = VpointY1 + 1;
				theta = tempY - (int)tempY;
			}

			if(VpointX1>9 || VpointY1>9)
			{
				targetView.setColor(c, r, unsigned char(rayRGB.x), unsigned char(rayRGB.y), unsigned char(rayRGB.z));
				continue;
			}


			viewImageList[(VpointY1-1)*9 + VpointX1-1].getColor(c,r,rayTempR1,rayTempG1,rayTempB1);
			viewImageList[(VpointY1-1)*9 + VpointX2-1].getColor(c,r,rayTempR2,rayTempG2,rayTempB2);
			viewImageList[(VpointY2-1)*9 + VpointX1-1].getColor(c,r,rayTempR3,rayTempG3,rayTempB3);
			viewImageList[(VpointY2-1)*9 + VpointX2-1].getColor(c,r,rayTempR4,rayTempG4,rayTempB4);

			bindR1 = rayTempR1*(1-alpha) + rayTempR2*alpha;
			bindG1 = rayTempG1*(1-alpha) + rayTempG2*alpha;
			bindB1 = rayTempB1*(1-alpha) + rayTempB2*alpha;

			bindR2 = rayTempR3*(1-alpha) + rayTempR4*alpha;
			bindG2 = rayTempG3*(1-alpha) + rayTempG4*alpha;
			bindB2 = rayTempB3*(1-alpha) + rayTempB4*alpha;

			rayRGB.x = bindR1*(1-theta) +
			 bindR2*(theta);
			rayRGB.y = bindG1*(1-theta) + bindG2*(theta);
			rayRGB.z = bindB1*(1-theta) + bindB2*(theta);
			
			//! record the resampled pixel value
			targetView.setColor(c, r, unsigned char(rayRGB.x), unsigned char(rayRGB.y), unsigned char(rayRGB.z));
		}
	}
	string savePath = "newView.bmp";
	targetView.save(savePath.c_str());
	cout << "Result saved!" << endl;
	
	return 0;
}
