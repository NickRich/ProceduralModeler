#include "Cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"

float noise[200][200];

Cloud::Cloud()
{

}

void Cloud::setValues(int xPos, int yPos, float darkness, int width, int length)
{
	this->xPos = xPos;
	this->yPos = yPos;
	this->darkness = darkness;
	this->width = width;
	this->length = length;
}

float** Cloud::genNoise()
{

	for (int x = 0; x < 200; x++)
	{
		for (int y = 0; y < 200; y++)
		{
			noise[x][y] = rand();
		}
	}

	float ** noisePointer = (float**)noise;
	return noisePointer;
}



Cloud::~Cloud()
{

}

