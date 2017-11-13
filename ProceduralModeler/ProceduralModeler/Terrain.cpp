#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Terrain.h"


Terrain::Terrain(int winWidth, float s, bool mountains)
{
	width = winWidth;
	roughnessFactor = s;
	genMountains = mountains;
}

float Terrain::generateRandomOffset(float aX, float aY, float bX, float bY, float s)
{
	//Code from slides
	float x1;
	float x2;
	float w;

	do
	{
		x1 = 2.0 * 0.001 * (float)(rand() % 1000) - 1.0;
		x2 = 2.0 * 0.001 * (float)(rand() % 1000) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.0);

	w = sqrt((-2.0 * log(w)) / w);

	float y1 = x1 * w;
	float y2 = x2 * w;

	float posX = abs((long)(bX - aX));
	float posY = abs((long)(bY - aY));

	float afterGauss = y1 * posX + y2 * posY;

	return s * afterGauss;
}

float Terrain::generateMidpoint(float a, float b, float r)
{
	//pass in y(a) as a and y(b) as b, r is randomOffset
	//return (y(a)+y(b))/2 + r
	float retValue = ((a + b) / 2 + r);
	//float retValue = (a + b) / 2.0;
	return retValue;
}

void Terrain::makePicture()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < width; j++)
		{
			pixels[i][j] = 0.0;
		}
	}

	for (int i = 0; i < width; i++)
	{
		pixels[i][(int)floor(heights[i])] = 1.0;
	}
}

void Terrain::calcMidpoints(int leftX, int leftY, int rightX, int rightY)
{
	if (rightX - leftX <= 1)
	{
		return;
	}
	float r = Terrain::generateRandomOffset(leftX, leftY, rightX, rightY, .001);
	float midY = Terrain::generateMidpoint(leftY, rightY, r);
	int midX = (leftX + rightX) / 2;

	heights[midX] = midY;

	calcMidpoints(leftX, leftY, midX, midY);
	calcMidpoints(midX, midY, rightX, rightY);
}

void Terrain::generateEndpoints()
{
	//These numbers are by no means set in stone,
	//I'm just trying to get my thoughts in order

	float leftEndpointY;
	float rightEndpointY;
	if (genMountains)
	{
		leftEndpointY = rand() % 500;
		rightEndpointY = rand() % 500;
	}
	else
	{
		leftEndpointY = rand() % 100;
		rightEndpointY = rand() % 100;
	}
	heights[0] = leftEndpointY;
	heights[width] = rightEndpointY;
}