#include "Cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
using namespace std;

float noise[500][500];

Cloud::Cloud()
{
	genNoise();
}

void Cloud::setValues(int xPos, int yPos, float darkness, int width, int length)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->darkness = darkness;
	this->width = width;
	this->length = length;

}

float Cloud::smoothNoise(float x, float y)
{
	float value = 0.0;

	//get number after the decimal point
	float decimalX = x - int(x);
	float decimalY = y - int(y);

	//get surrounding pixels
	int x1 = (int(x) + 500) % 500;
	int y1 = (int(y) + 500) % 500;
	int x2 = (x1 + 500 - 1) % 500;
	int y2 = (y1 + 500 - 1) % 500;

	//smooth with bilinear interpolation
	value += decimalX * decimalY * noise[y1][x1];
	value += (1 - decimalX) * decimalY * noise[y1][x2];
	value += decimalX * (1 - decimalY) * noise[y2][x1];
	value += (1 - decimalX) * (1 - decimalY) * noise[y2][x2];

	return value;
	
}

void Cloud::genNoise()
{

	for (int x = 0; x < 500; x++)
	{
		for (int y = 0; y < 500; y++)
		{
			
			noise[x][y] = (rand() % 32768) / 32768.0;
			
		}
	}  


	//for (int x = 0; x < 500; x++)
	//{
	//	for (int y = 0; y < 500; y++)
	//	{

	//		noise[x][y] = smoothNoise(x,y);

	//	}
	//}

	//printf("noise %f", noise[0][1]);
}



Cloud::~Cloud()
{

}

