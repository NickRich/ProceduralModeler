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
float leftEndpoint;
float rightEndpoint;

void generateEndpoints()
{
	//These numbers are by no means set in stone,
	//I'm just trying to get my thoughts in order

	if (generatingMountains)
	{
		leftEndpoint = rand() % 500;
		rightEndpoint = rand() % 500;
	}
	else
	{
		leftEndpoint = rand() % 100;
		rightEndpoint = rand() % 100;
	}
}



void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	Cloud * c = new Cloud();
	c->genNoise();
	glutSwapBuffers();

	//Split area by 2 every time until we cover the whole thing
	//Calculate the midpoint at each step

}

/**
*    Main function
*/
int main(int argc, char **argv)
{
	srand(time(NULL));
	t = new Terrain();

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL/FreeGLUT - Example: Rendering a textured .obj model using shaders");

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
