#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Terrain.h"



float Terrain::generateRandomOffset()
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
	//y1 = x1 * w;
	//y2 = x2 * w;

	return 0.0;
}

float Terrain::generateMidpoint(float a, float b, float r)
{
	//pass in y(a) as a and y(b) as b, r is randomOffset
	//return (y(a)+y(b))/2 + r
	return (a + b) / 2 + r;
}
