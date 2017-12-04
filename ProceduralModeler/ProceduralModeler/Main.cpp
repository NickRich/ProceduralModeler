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

int winHeight = 500;
int winWidth = 500;
int scale = 4;
int cols = 1024 / scale;
int rows = 1024 / scale;

int lightX = 0;
int lightY = 0;


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
	{-525, -200, -1200, 1}
};

//GLfloat ambient[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f};
//GLfloat specular[] = { 0.5f, 0.5f, 0.5f, 1.0f };
//GLfloat position[] = {-500.0f, -500.0f, -1000.0f, 1.0f };

void drawTerrain3D()
{
	glClearColor(0.2, 0.6, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 1, -500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)myModelMat);

	glEnableClientState(GL_VERTEX_ARRAY);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	// Create light components
	float ambientLight[] = { 0.6, 0.6, 0.6, 1.0 };
	float diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	float specularLight[] = { 0.5, 0.5, 0.5, 1.0 };
	float position[] = { 500, 400, 1100, 1.0 };

	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	glLightfv(GL_LIGHT0, GL_POSITION, position);

	int color;

	for (int z = 1; z < rows; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 1; x < cols; x++)
		{
			color = rand() % 2;
			if (color == 0)
			{
				glColor3f(0.137255, 0.556863, 0.137255);
			}
			else if (color == 1)
			{
				glColor3f(0.184314, 0.309804, 0.184314);
			}
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f(x *scale, t->terrain[z * scale][x * scale], z * scale);
			glVertex3f((x + 1) * scale, t->terrain[z * scale][(x + 1) * scale], z * scale);
			glVertex3f(x * scale, t->terrain[(z + 1) * scale][x * scale], (z + 1) * scale);
		}
		glEnd();
	}

	for (int z = 1; z < rows; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 1; x < cols; x++)
		{
			color = rand() % 2;
			if (color == 0)
			{
				glColor3f(0.137255, 0.556863, 0.137255);
			}
			else if (color == 1)
			{
				glColor3f(0.184314, 0.309804, 0.184314);
			}
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f((x + 1) * scale, t->terrain[z * scale][(x + 1) * scale], z * scale);
			glVertex3f(x * scale, t->terrain[(z + 1) * scale][x * scale], (z + 1) * scale);
			glVertex3f((x + 1) * scale, t->terrain[(z + 1) * scale][(x + 1) * scale], (z + 1) * scale);
		}
		glEnd();
	}

	//for (int z = 1; z < rows; z++)
	//{
	//	glBegin(GL_LINES);
	//	for (int x = 1; x < cols; x++)
	//	{
	//		glVertex3f(x*scale, t->terrain[z*scale][x*scale], z*scale);
	//		glVertex3f((x+1)*scale, t->terrain[z*scale][(x+1)*scale], z*scale);
	//	}
	//	glEnd();
	//}

	//for (int z = 1; z < rows; z++)
	//{
	//	glBegin(GL_LINES);
	//	for (int x = 1; x < cols; x++)
	//	{
	//		glVertex3f(x*scale, t->terrain[z*scale][x*scale], z*scale);
	//		glVertex3f(x*scale, t->terrain[(z+1)*scale][x*scale], (z+1)*scale);
	//	}
	//	glEnd();
	//}
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
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
	glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CS 334 - Procedural Modeling");

	srand(time(NULL));
	t = new Terrain(rightEndpointX, generatingMountains);

	//need something to start with what kind of terrain we're generating
//	t->generateEndpoints();
	//calculate values, store in array
//	t->calcMidpoints(leftEndpointX, t->heights[leftEndpointX], rightEndpointX, t->heights[rightEndpointX], roughnessFactor);
//	t->makePicture();

	t->generateEndpoints3D();
	t->TerrainGenerate(1024);
//	t->printHeights();

	glEnable(GL_NORMALIZE);



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
