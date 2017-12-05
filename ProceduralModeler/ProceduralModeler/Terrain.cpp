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
	terrain[0][0] = rand() % 200;
	terrain[0][1024] = rand() % 200;
	terrain[1024][0] = rand() % 1;
	terrain[1024][1024] = rand() % 1;
}

float Terrain::Offset3D(int aX, int aY, int aZ, int bX, int bY, int bZ, float s)
{
	float x1;
	float x2;
	float x3;
	float w;

	do
	{
		x1 = 2.0 * 0.001 * (float)(rand() % 1000) - 1.0;
		x2 = 2.0 * 0.001 * (float)(rand() % 1000) - 1.0;
		x3 = 2.0 * 0.001 * (float)(rand() % 1000) - 1.0;
		w = x1 * x1 + x2 * x2;
	} while (w >= 1.0);

	float y1 = x1 * w;
	float y2 = x2 * w;
	float y3 = x3 * w;

	float posX = abs((long)(bX - aX));
	float posY = abs((long)(bY - aY));
	float posZ = abs((long)(bZ - aZ));

	float afterGauss = y1 * posX + y2 * posY + y3 * posZ;

	return s * afterGauss;
}

void Terrain::TerrainGenerate(int range, float roughness)
{
	/*
	a---f---b
	|       |
	i   e   g
	|       |
	c---h---d
	*/

	if (range < 1) return;

	for (int i = 0; i < 1024; i += range)
	{
		for (int j = 0; j < 1024; j += range)
		{
			float a = terrain[i][j];
			float b = terrain[i][j + range];
			float c = terrain[i + range][j];
			float d = terrain[i + range][j + range];

			int eZ = i + range / 2;
			int eX = j + range / 2;

			float e = (a + b + c + d) / 4;
			terrain[eZ][eX] = e + Offset3D(j, a, i, j + range, d, i + range, roughness);// +generateRandomOffset(j, a, j + range, d, .03);

			int fX = eX;
			int fZ = i;

			int gX = j + range;
			int gZ = eZ;

			int hX = eX;
			int hZ = i + range;

			int iX = j;
			int iZ = eZ;

			float f = (a + b + e) / 3 + Offset3D(j, a, i, j + range, b, i, roughness); // +generateRandomOffset(j, a, j + range, b, .03);
			float g = (b + d + e) / 3 + Offset3D(j + range, b, i, j + range, d, i + range, roughness);// +generateRandomOffset(i, b, i + range, d, .03);
			float h = (d + c + e) / 3 + Offset3D(j, c, i + range, j + range, d, i + range, roughness);// +generateRandomOffset(j, c, j + range, d, .03);
			float iVal = (c + a + e) / 3 + Offset3D(j, a, i, j, c, i + range, roughness);// +generateRandomOffset(i, a, i + range, c, .03);

			terrain[fZ][fX] = f;
			terrain[gZ][gX] = g;
			terrain[hZ][hX] = h;
			terrain[iZ][iX] = iVal;

		}
	}

	TerrainGenerate(range / 2, roughness);
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