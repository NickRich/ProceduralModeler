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
#include <vector>
using namespace std;

#include "GL/glew.h"
#include "GL/glut.h"
#include "Cloud.h"
#include "Terrain.h"

Terrain * t;
Cloud * c;
vector<Cloud *> cloudList;
bool generatingMountains = true;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointY; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointX = 499; //Holds value highest x position (width of screen)
float roughnessFactor; //Roughness Factor for calculating random variable, defined by user
int heights[500]; //used to store heights for each 

GLuint cloudTex;
GLuint fragmentShader = 0;

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
	heights[leftEndpointX] = leftEndpointY;
	heights[rightEndpointX] = rightEndpointY;
}

void calcMidpoints(int leftX, int leftY, int rightX, int rightY)
{
	if (rightX - leftX <= 1)
	{
		return;
	}
	float r = t->generateRandomOffset(leftX, leftY, rightX, rightY, roughnessFactor);
	float midY = t->generateMidpoint(leftY, rightY, r);
	int midX = (leftX + rightX) / 2;

	heights[midX] = midY;

	calcMidpoints(leftX, leftY, midX, midY);
	calcMidpoints(midX, midY, rightX, rightY);
}

void drawCloud(Cloud * c)
{
	/* Draw clouds*/

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 500.0, 0.0);

	for (int x = 0; x < 500; x++)
	{
		for (int y = 0; y < 500; y++)
		{
			float pixNoise = c->getNoise(x, y);
			glBegin(GL_POINTS);
			glColor3f(pixNoise, pixNoise, pixNoise);
			glVertex2i(x, y);
			glEnd();
		}
	}

	glutSwapBuffers();
	glFlush();
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Split area by 2 every time until we cover the whole thing
	//Calculate the midpoint at each step

	//draw clouds from the list
	for(int i = 0; i < cloudList.size(); i++)
	{
		Cloud * cloud = cloudList.at(i);
		drawCloud(cloud);
		
	}

	
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
	t = new Terrain();
	//create a new cloud and add it to the list of clouds
	c = new Cloud();
	cloudList.push_back(c);

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("OpenGL/FreeGLUT - Example: Rendering a textured .obj model using shaders");

	srand(time(NULL));
	t = new Terrain();
	roughnessFactor = 1;

	//need something to start with what kind of terrain we're generating
	generateEndpoints();

	//calculate values, sotre in array
	calcMidpoints(leftEndpointX, leftEndpointY, rightEndpointX, rightEndpointY);

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
