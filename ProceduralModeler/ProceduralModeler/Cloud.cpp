#include "Cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
using namespace std;

float noise[100][100];

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

float Cloud::getNoise(int x , int y)
{
	return noise[x][y];
}

void Cloud::genNoise()
{

	for (int x = 0; x < 100; x++)
	{
		for (int y = 0; y < 100; y++)
		{
			
			noise[x][y] = (rand() % 32768) / 32768.0;
			

		}
	}
	//printf("noise %f", noise[0][1]);
}



Cloud::~Cloud()
{

}

