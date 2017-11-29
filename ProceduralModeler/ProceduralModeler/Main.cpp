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
bool generatingDesert = false;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointY; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointX = 500; //Holds value highest x position (width of screen)
float roughnessFactor = 0.08; //Roughness Factor for calculating random variable, defined by user

int winHeight = 513;
int winWidth = 513;
int scale = 16;
int cols = 513 / 16;
int rows = 513 / 16;

void drawTerrain()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 500.0, 0.0);

	for (int i = 0; i < 500; i++)
	{
		for (int j = 0; j < 500; j++)
		{
			float pixNoise = t->pixels[i][j];
			glBegin(GL_POINTS);
			if (pixNoise == 1.0) {
				if (j < 125)
				{
					int gray = 1;
					if (j > 110)
					{
						gray = rand() % 2;
					}
					else if (j > 108)
					{
						gray = rand() % 5;
					}
					else if (j > 100)
					{
						gray = rand() % 10;
					}
					if (gray == 0)
					{
						glColor3f(0.5, 0.5, 0.5);
					}
					else
					{
						glColor3f(1.0, 1.0, 1.0);
					}
				}
				else if (j < 300)
				{
					int white = 1;
					int green = 1;
					if (j < 130)
					{
						white = rand() % 2;
					}
					else if (j < 140)
					{
						white = rand() % 5;
					}
					else if (j < 150)
					{
						white = rand() % 10;
					}

					if (j > 290)
					{
						green = rand() % 2;
					}
					else if (j > 280)
					{
						green = rand() % 5;
					}
					else if (j > 275)
					{
						green = rand() % 10;
					}
					if (white == 0)
					{
						glColor3f(1.0, 1.0, 1.0);
					}
					else if (green == 0)
					{
						glColor3f(0.0, 1.0, 0.0);
					}
					else
					{
						glColor3f(0.5, 0.5, 0.5);
					}
				}
				else
				{
					if (generatingDesert)
					{
						int darkBrown = rand() % 15;
						if (darkBrown == 0)
						{
							glColor3f(0.36, 0.25, 0.31);
						}
						else
						{
							glColor3f(0.86, 0.58, 0.44);
						}
					}
					else
					{
						int gray = 1;
						if (j < 310)
						{
							gray = rand() % 2;
						}
						else if (j < 320)
						{
							gray = rand() % 5;
						}
						else if (j < 325)
						{
							gray = rand() % 10;
						}
						if (gray == 0)
						{
							glColor3f(0.5, 0.5, 0.5);
						}
						else
						{
							glColor3f(0.0, 1.0, 0.0);
						}
					}
				}
			}
			else
			{
				glColor3f(0.2, 0.6, 0.8);
			}
			glVertex2i(i, j);
			glEnd();
		}
	}

	glutSwapBuffers();
	glFlush();
}

void drawClouds()
{
	/* Draw clouds*/
	//draw clouds from the list
	

	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 500.0, 500.0, 0.0);

	//for each pixel
	for (int x = 0; x < 500; x++)
	{

		for (int y = 0; y < 500; y++)
		{
			float pixColor = 0.0;
			float zoom = 256.0;
			float darkness = 0.5;
			//add all clouds colors at the pixel
			for (int i = 0; i < cloudList.size(); i++)
			{
				//get cloud
				Cloud * cloud = cloudList.at(i);
				//get smoothened and zoomed noise value
				float pixNoise = c->smoothNoise(x / zoom, y / zoom);
				//add to color but make the following clouds darker
				pixColor += pixNoise/darkness;
	
				//zoom in
				zoom /= 2.0;
				//darken
				darkness *= 2;
			}
			//draw the pixel
			glBegin(GL_POINTS);
			glColor3f(pixColor/5, pixColor/5, pixColor/5);
			glVertex2i(x, y);
			glEnd();
		}
	}

	//flush all changes
	glutSwapBuffers();
	glFlush();
}

GLfloat myModelMat[4][4] =
{
	{1, 0, 0, 0},
	{0, 1, 0, 0},
	{0, 0, 1, 0},
	{-17, -5, -10, 1}
};

float floorVertices[12] =
{
	-512, 0, -512,
	-1, 0, 9,
	9, 0, 9,
	9, 0, -1
};

void drawTerrain3D()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 1, -300);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)myModelMat);

	glEnableClientState(GL_VERTEX_ARRAY);

	for (int i = -rows; i < rows; i++)
	{
		for (int j = -cols; j < cols; j++)
		{
			glBegin(GL_TRIANGLES);
			glVertex3f(i, t->terrain[i][j], j);
			glVertex3f(i + 1, t->terrain[i+1][j], j);
			glVertex3f(i, t->terrain[i][j+1], j + 1);
			glEnd();
		}
	}

	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
	glFlush();
}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/*Draw Clouds comment out if need be*/
	//drawClouds();

	//drawTerrain();
	drawTerrain3D();
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
	cloudList.push_back(c);
	c = new Cloud();
	cloudList.push_back(c);
	c = new Cloud();
	cloudList.push_back(c);
	c = new Cloud();
	cloudList.push_back(c);
	c = new Cloud();
	cloudList.push_back(c);

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(513, 513);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CS 334 - Procedural Modeling");

	srand(time(NULL));
	t = new Terrain(rightEndpointX, generatingMountains);

	//need something to start with what kind of terrain we're generating
	t->generateEndpoints();
	//calculate values, store in array
	t->calcMidpoints(leftEndpointX, t->heights[leftEndpointX], rightEndpointX, t->heights[rightEndpointX], roughnessFactor);
	t->makePicture();

	t->generateEndpoints3D();
	t->diamondDivision(0, 512, 512, 512, 0, 0, 512, 0);
	//t->printHeights();

	glutDisplayFunc(display);
//	glutIdleFunc(idle);

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
