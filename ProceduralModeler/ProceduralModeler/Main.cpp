/**
*	CS 334 - Fundamentals of Computer Graphics
*	Assignment 3 Framework
*
*	Important: The .obj file should contain triangle information instead of polygons
*
*	Instructions:
*	- Press ESC to exit
*/

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>

#include "GL/glew.h"
#include "GL/glut.h"
#include "Cloud.h"
#include "Terrain.h"

Terrain * t;
bool generatingMountains = true;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointX; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointY = 499; //Holds value highest x position (width of screen)
float roughnessFactor; //Roughness Factor for calculating random variable, defined by user
int heights[500]; //used to store heights for each 

void generateEndpoints()
{
	//These numbers are by no means set in stone,
	//I'm just trying to get my thoughts in order

	if (generatingMountains)
	{
		leftEndpointY = rand() % 500;
		rightEndpointY = rand() % 500;
	}
	else
	{
		leftEndpointY = rand() % 100;
		rightEndpointY = rand() % 100;
	}
}

void calcMidpoints(int leftX, int leftY, int rightX, int rightY)
{
	if (rightX - leftX <= 1)
	{
		return;
	}
	float r = t->generateRandomOffset(leftX, leftY, rightX, rightY, roughnessFactor);
	int midY = t->generateMidpoint(leftY, rightY, r);
	int midX = (leftX + rightX) / 2;

	heights[midX] = midY;

	calcMidpoints(leftX, leftY, midX, midY);
	calcMidpoints(midX, midY, rightX, rightY);
}


void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	Cloud * c = new Cloud();
	c->genNoise();

	for (int i = 0; i < rightEndpointX - 1; i++)
	{
		int line[4] =
		{
			i,
			heights[i],
			i + 1,
			heights[i + 1]
		};
		glVertexPointer(2, GL_FLOAT, 0, line);
		glDrawArrays(GL_LINES, 0, 2);
	}

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	glFlush();

	glutSwapBuffers();
}

/**
*    Main function
*/
int main(int argc, char **argv)
{

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL/FreeGLUT - Example: Rendering a textured .obj model using shaders");

	srand(time(NULL));
	t = new Terrain();

	//calculate values, sotre in array
	calcMidpoints(leftEndpointX, leftEndpointY, rightEndpointX, rightEndpointY);

	glutDisplayFunc(display);

	/* Init GLEW */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

	//need something to start with what kind of terrain we're generating
	generateEndpoints();

  
    /* Start the main GLUT loop */
    /* NOTE: No code runs after this */
    glutMainLoop();
}
