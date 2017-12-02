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
#include "Tree.h"

Terrain * t;
Cloud * c;
vector<Cloud *> cloudList;
vector<Tree *> treeList;
bool generatingMountains = true;
bool generatingDesert = false;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointY; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointX = 500; //Holds value highest x position (width of screen)
float roughnessFactor = 0.1; //Roughness Factor for calculating random variable, defined by user

void drawTerrain()
{
	glClear(GL_COLOR_BUFFER_BIT);
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
				if (j < 100)
				{
					glColor3f(1.0, 1.0, 1.0);
				}
				else if (j < 250)
				{
					glColor3f(0.5, 0.5, 0.5);
				}
				else
				{
					if (generatingDesert)
					{
						glColor3f(0.86, 0.58, 0.44);
					}
					else {
						glColor3f(0.0, 1.0, 0.0);
					}
				}
			}
			else
			{
				glColor3f(0.0, 0.5, 0.8);
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

void genTree()
{
	Tree * tree;
	tree = new Tree(0.0, 0.0, 0.0);
	tree->genBranches(tree);
	treeList.push_back(tree);
}

void drawTrees()
{
	//for each tree
	for (int t = 0; t < treeList.size(); t++)
	{
		Tree * tree = treeList.at(t);
		vector <Tree *> branchList = tree->branches;
		
		//for each branch in a tree
		for (int a = 0; a < branchList.size(); a++)
		{
			//get a branch
			Tree * branch = branchList.at(a);

			//get angles
			float angleX = branch->angleX;
			float angleY = branch->angleY;
			float angleZ = branch->angleZ;

			//get location
			float x = branch->x;
			float y = branch->y;
			float z = branch->z;
			//get radius and height
			float radiusB = branch->radiusBottom;
			float radiusT = branch->radiusTop;
			float height = branch->height;
			//create a cylinder
			GLUquadric *obj = gluNewQuadric();
			//brown color
			glColor3f(0.5, 0.35, 0.1);
			//if smallest branch, make it a leaf
			if (branch->height <= 0.3)
			{
				glColor3f(0.5, 0.6 , 0.1);
			}
			
			glPushMatrix();

			//position
			glTranslatef(x, y -1 , z);

			//flip
			glScalef(1.0f, -1.0f, 1.0f);

			glRotated(angleX, 1, 0, 0);
			glRotated(angleY, 0, 1, 0);
			glRotated(angleZ, 0, 0, 1);

			gluCylinder(obj, radiusB, radiusT, height, 30, 30);
			glPopMatrix();
			

		}
		glutSwapBuffers();
		glFlush();
	}
	
	
	

}

void display()
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//Split area by 2 every time until we cover the whole thing
	//Calculate the midpoint at each step

	/*Draw Clouds comment out if need be*/
	//drawClouds();

	//drawTerrain();
	drawTrees();
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

	//generate a tree
	genTree();

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
    glutInitWindowSize(500, 500);
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
