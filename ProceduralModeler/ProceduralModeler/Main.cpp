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
Cloud * c;
bool generatingMountains = true;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointY; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointX = 200; //Holds value highest x position (width of screen)
float roughnessFactor; //Roughness Factor for calculating random variable, defined by user


void display()
{
	//glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glutSwapBuffers();

	//Get clouds and draw with glDrawPixels

	/* Draw clouds*/
	glClear(GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glDrawBuffer(GL_FRONT_AND_BACK);

	float ** noise = c->genNoise();

	/* Step 2: Set up arrays and draw them */
	glDrawPixels(100, 100, GL_RGB, GL_FLOAT,noise);

//	glDrawPixels(500, 500, GL_RGB, GL_FLOAT, (float**)t->pixels);

	/* Disable the clients */
	glDrawBuffer(GL_BACK);
	glutSwapBuffers();
	glFlush();

}

/*redraw display*/
void idle()
{
	/* Redraw the window */
	glutPostRedisplay();
}

/**
*    Main function
*/
int main(int argc, char **argv)
{
	srand(time(NULL));
	c = new Cloud();
    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL/FreeGLUT - Example: Rendering a textured .obj model using shaders");

	srand(time(NULL));
	roughnessFactor = .001;
	t = new Terrain(rightEndpointX, roughnessFactor, true);

	//need something to start with what kind of terrain we're generating
	t->generateEndpoints();
	//calculate values, store in array
	t->calcMidpoints(leftEndpointX, t->heights[leftEndpointX], rightEndpointX, t->heights[rightEndpointX]);
	t->makePicture();

	glutDisplayFunc(display);
	glutIdleFunc(idle);

	/* Init GLEW */
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}
	std::cout << "GLEW version: " << glewGetString(GLEW_VERSION) << std::endl;

  
    /* Start the main GLUT loop */
    /* NOTE: No code runs after this */
    glutMainLoop();
}
