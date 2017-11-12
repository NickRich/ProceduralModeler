#include "Cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
//#include "GL/glui.h"

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

void Cloud::genNoise()
{

	for (int x = 0; x < 200; x++)
	{
		for (int y = 0; y < 200; y++)
		{
			noise[x][y] = rand();
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw the square */
	/* Step 1: Enable the clients for the vertex arrays */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	/* Step 2: Set up arrays and draw them */
	glDrawPixels(100, 100, GL_RGB, GL_FLOAT, noise);

	/* Step 3: Disable the clients */
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glFlush();
	Sleep(2000);
}



Cloud::~Cloud()
{

}

