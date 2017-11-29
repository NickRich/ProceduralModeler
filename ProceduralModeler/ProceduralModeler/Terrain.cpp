#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Terrain.h"


Terrain::Terrain(int winWidth, bool mountains)
{
	width = winWidth;
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
		int height = (int)floor(heights[i]);
		if (height > 500)
		{
			height = 500;
		}
		pixels[i][height] = 1.0;
	}
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			if (pixels[i][j] == 1)
			{
				for (int k = j; k < 500; k++)
				{
					pixels[i][k] = 1.0;
				}
			}
		}
	}
}

void Terrain::calcMidpoints(int leftX, int leftY, int rightX, int rightY, float s)
{
	if (rightX - leftX <= 1)
	{
		return;
	}
	float r = Terrain::generateRandomOffset(leftX, leftY, rightX, rightY, s);
	float midY = Terrain::generateMidpoint(leftY, rightY, r);
	int midX = (leftX + rightX) / 2;

	heights[midX] = midY;

	calcMidpoints(leftX, leftY, midX, midY, s);
	calcMidpoints(midX, midY, rightX, rightY, s);
}

void Terrain::generateEndpoints()
{
	//These numbers are by no means set in stone,
	//I'm just trying to get my thoughts in order

	float leftEndpointY;
	float rightEndpointY;
	if (genMountains)
	{
		leftEndpointY = rand() % 300;
		rightEndpointY = rand() % 300;
	}
	else
	{
		leftEndpointY = rand() % 100 + 300;
		rightEndpointY = rand() % 100 + 300;
	}
	heights[0] = leftEndpointY;
	heights[width] = rightEndpointY;
}

void Terrain::generateEndpoints3D()
{
	for (int i = 0; i < 513; i++)
	{
		for (int j = 0; j < 513; j++)
		{
			terrain[i][j] = 0;
		}
	}
	terrain[0][0] = (rand() % 500) / 100;
	terrain[0][512] = (rand() % 500) / 100;
}

void Terrain::squareDivisionUp(int rightX, int rightZ, int downX, int downZ, int leftX, int leftZ)
{
	float height1 = terrain[rightZ][rightX];
	float height2 = terrain[downZ][downX];
	float height3 = terrain[leftZ][leftX];

	float average = (height1 + height3) / 2;
	terrain[leftZ][downX] = average;

}

void Terrain::squareDivisionRight(int downX, int downZ, int leftX, int leftZ, int upX, int upZ)
{
	float height1 = terrain[downZ][downX];
	float height2 = terrain[leftZ][leftX];
	float height3 = terrain[upZ][upX];

	float average = (height1 + height3) / 2;
	terrain[leftZ][downX] = average;
}

void Terrain::squareDivisionDown(int leftX, int leftZ, int upX, int upZ, int rightX, int rightZ)
{
	float height1 = terrain[leftZ][leftX];
	float height2 = terrain[upZ][upX];
	float height3 = terrain[rightZ][rightX];

	float average = (height1 + height3) / 2;
	terrain[rightZ][upX] = average;
}

void Terrain::squareDivisionLeft(int upX, int upZ, int rightX, int rightZ, int downX, int downZ)
{
	float height1 = terrain[upZ][upX];
	float height2 = terrain[rightZ][rightX];
	float height3 = terrain[downZ][downX];

	float average = (height1 + height3) / 2;
	terrain[rightZ][upX] = average;
}

//Use 4 corner points to calculate height
void Terrain::diamondDivision(int bottomLeftX, int bottomLeftZ, int bottomRightX, int bottomRightZ, int topLeftX, int topLeftZ, int topRightX, int topRightZ)
{
	if (bottomRightX - bottomLeftX <= 1 || bottomLeftZ - topLeftZ <= 1) return;

	float average = terrain[bottomLeftZ][bottomLeftX] + terrain[bottomRightZ][bottomRightX] + terrain[topLeftZ][topLeftX] + terrain[topRightZ][topRightX];
	average /= 4;
	int centerX = (bottomLeftX + bottomRightX) / 2;
	int centerZ = (bottomLeftZ + topLeftZ) / 2;
	terrain[centerX][centerZ] = average;
	squareDivisionUp(topRightX, topRightZ, centerX, centerZ, topLeftX, topLeftZ);
	squareDivisionRight(bottomRightX, bottomRightZ, centerX, centerZ, topRightX, topRightZ);
	squareDivisionDown(bottomLeftX, bottomLeftZ, centerX, centerZ, bottomRightX, bottomRightZ);
	squareDivisionLeft(topLeftX, topLeftZ, centerX, centerZ, bottomLeftX, bottomLeftZ);

	int leftX = topLeftX;
	int upZ = topLeftZ;
	int rightX = bottomRightX;
	int downZ = bottomRightZ;

	//Up Left Corner
	diamondDivision(leftX, centerZ, centerX, centerZ, topLeftX, topLeftZ, centerX, upZ);
	//Up Right Corner
	diamondDivision(centerX, centerZ, rightX, centerZ, centerX, upZ, topRightX, topRightZ);
	//Bottom Right Corner
	diamondDivision(centerX, downZ, bottomRightX, bottomRightZ, centerX, centerZ, rightX, centerZ);
	//Bottom Left Corner
	diamondDivision(bottomLeftX, bottomLeftZ, centerX, downZ, leftX, centerZ, centerX, centerZ);
}

void Terrain::printHeights()
{
	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			printf("terrain[%i][%i], %f\n", i, j, terrain[i][j]);
		}
	}
}