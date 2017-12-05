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
#include "Cactus.h"

Terrain * t;
Cloud * c;
vector<Cloud *> cloudList;
vector<Tree *> treeList;
vector<Cactus *> cactusList;
bool generatingMountains = true;
bool generatingDesert = false;
int leftEndpointY; //Holds value for height of leftEndpoint
int rightEndpointY; //Holds value for height of rightEndpoint
int leftEndpointX = 0; //Holds value for least x position (start of screen)
int rightEndpointX = 500; //Holds value highest x position (width of screen)
float roughnessFactor = 0.1; //Roughness Factor for calculating random variable, defined by user
int winWidth = 500;
int winHeight = 500;

int scale = 1;
int rows = 1024 / scale;
int cols = 1024 / scale;

GLfloat modelMat[4][4] = {
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{ 0, -1, -26, 1 }
};

GLfloat modelMatC[4][4] = {
	{ 1, 0, 0, 0 },
	{ 0, 1, 0, 0 },
	{ 0, 0, 1, 0 },
	{-500, -500, -1200, 1 }
};

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

	//glutSwapBuffers();
	//glFlush();
}


void genCactus(float x, float y, float z)
{
	Cactus* cactus;
	cactus = new Cactus(x, y, z);
	cactus->genBranches(cactus);
	cactusList.push_back(cactus);
}

void drawCactus()
{
	float fovy = 90.0;
	float aspect = 500.0 / 500.0;
	float zNear = 1.0;
	float zFar = 100.0;

	//perspective projection
	///* Clear the window */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)modelMat);


	//set up lighting effects
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	//// Create light components
	//float ambientLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//float diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//float specularLight[] = { 0.5, 0.5, 0.5, 1.0 };
	//float position[] = { 1.0, 1.0, 0.0, 1.0 };

	//// Assign created components to GL_LIGHT0
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);




	//for each tree
	for (int t = 0; t < cactusList.size(); t++)
	{
		Cactus * cactus = cactusList.at(t);
		vector <Cactus *> branchList = cactus->branches;

		//for each branch in a tree
		for (int a = 0; a < branchList.size(); a++)
		{
			//get a branch
			Cactus * branch = branchList.at(a);

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
			glColor3f(0.5, 0.6, 0.1);
	

			glPushMatrix();

			//position
			glTranslatef(x, y - 1, z);

			//flip
			glScalef(1.0f, -1.0f, 1.0f);

			glRotated(angleX, 1, 0, 0);
			glRotated(angleY, 0, 1, 0);
			glRotated(angleZ, 0, 0, 1);

			//tappered cylinder 
			gluCylinder(obj, radiusB, radiusT, height, 30, 30);
			glPopMatrix();
		

		}
		glScaled(1, 1, 1);
	}

	//disable lighting effects and flush buffers
	/*glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);*/
	//glutSwapBuffers();
	//glFlush();

}

void genTree(float x, float y, float z)
{
	Tree * tree;
	tree = new Tree(x, y, z);
	tree->genBranches(tree);
	treeList.push_back(tree);
}

void drawTrees()
{
	float fovy = 90.0;
	float aspect = 500.0 / 500.0;
	float zNear = 1.0;
	float zFar = 100.0;

	//perspective projection
	///* Clear the window */
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, aspect, zNear, zFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)modelMat);


	//set up lighting effects
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);

	//glColorMaterial(GL_FRONT, GL_DIFFUSE);
	//glEnable(GL_COLOR_MATERIAL);

	//// Create light components
	//float ambientLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//float diffuseLight[] = { 0.8, 0.8, 0.8, 1.0 };
	//float specularLight[] = { 0.5, 0.5, 0.5, 1.0 };
	//float position[] = { 1.0, 1.0, 0.0, 1.0 };

	//// Assign created components to GL_LIGHT0
	//glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	//glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	//glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
	//glLightfv(GL_LIGHT0, GL_POSITION, position);

	


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
			if (branch->height <= 0.0)
			{
				glPushMatrix();

				//position just above tallest branches
				glTranslatef(x, y - 1 + 0.1, z);

				//flip
				glScalef(1.0f, -1.0f, 1.0f);
				glColor3f(0.5, 0.6 , 0.1);
				
				glRotated(angleX, 1, 0, 0);
				glRotated(angleY, 0, 1, 0);
				glRotated(angleZ, 0, 0, 1);

				//make leaves as spheres
				gluSphere(obj, 20*radiusB, 30, 30);
				glPopMatrix();
			}
			//if tall enough make a branch
			else
			{

				glPushMatrix();

				//position
				glTranslatef(x, y - 1, z);

				//flip
				glScalef(1.0f, -1.0f, 1.0f);

				glRotated(angleX, 1, 0, 0);
				glRotated(angleY, 0, 1, 0);
				glRotated(angleZ, 0, 0, 1);

				//tappered cylinder 
				gluCylinder(obj, radiusB, radiusT, height, 30, 30);
				glPopMatrix();
			}

		}
		glScaled(1,1, 1);
	}
	
	//disable lighting effects and flush buffers
	/*glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);*/
	//glutSwapBuffers();
	//glFlush();

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
	
	/*glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(30, 1, 1, -500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf((GLfloat *)myModelMat);

	glEnableClientState(GL_VERTEX_ARRAY);



	int color;

	for (int z = 1; z < rows; z++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int x = 1; x < cols; x++)
		{
			color = rand() % 5;

			if (generatingDesert)
			{
				glColor3f(0.86, 0.58, 0.44);
				if (color == 0)
				{
					glColor3f(0.5, 0.35, 0.1);
				}
			}
			else
			{
				glColor3f(0.137255, 0.556863, 0.137255);
				if (color == 0)
				{
					glColor3f(0.184314, 0.309804, 0.184314);
				}

				if (t->terrain[z * scale][(x + 1) * scale] > 150)
				{
					glColor3f(0.35 + .05 * color, 0.35 + .05 * color, 0.35 + .05 * color);
				}
				else if (t->terrain[z * scale][(x + 1) * scale] > 250)
				{
					glColor3f(1.0, 1.0, 1.0);
				}
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
			color = rand() % 5;
			if (generatingDesert)
			{
				glColor3f(0.86, 0.58, 0.44);
				if (color == 0)
				{
					glColor3f(0.5, 0.35, 0.1);
				}
			}
			else
			{
				glColor3f(0.137255, 0.556863, 0.137255);
				if (color == 0)
				{
					glColor3f(0.184314, 0.309804, 0.184314);
				}
				if (generatingMountains)
				{
					if (t->terrain[z * scale][(x + 1) * scale] > 150)
					{
						glColor3f(0.35 + .05 * color, 0.35 + .05 * color, 0.35 + .05 * color);
					}
					else if (t->terrain[z * scale][(x + 1) * scale] > 250)
					{
						glColor3f(1.0, 1.0, 1.0);
					}
				}
			}
			glNormal3f(0.0f, 0.0f, 1.0f);
			glVertex3f((x + 1) * scale, t->terrain[z * scale][(x + 1) * scale], z * scale);
			glVertex3f(x * scale, t->terrain[(z + 1) * scale][x * scale], (z + 1) * scale);
			glVertex3f((x + 1) * scale, t->terrain[(z + 1) * scale][(x + 1) * scale], (z + 1) * scale);
		}
		glEnd();
	}

	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	glDisableClientState(GL_VERTEX_ARRAY);
	//
	//glutSwapBuffers();
	//glFlush();
}

void drawClouds()
{
	/* Draw clouds*/
	//draw clouds from the list
	

	//glClearColor(0.2, 0.6, 0.8, 1.0);
	/*glClear(GL_COLOR_BUFFER_BIT);*/
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, 1, 10, -10);
	//
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
	//glLoadMatrixf((GLfloat *)modelMatC);
	gluOrtho2D(0, 500, 500, 0);
	glDisable(GL_DEPTH);

	//for each pixel
	for (int x = 0; x < winWidth; x++)
	{

		for (int y = 0; y < winHeight; y++)
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
				zoom /= 2.0;
				//darken
				darkness *= 2;
			}
			pixColor /= 5;
			 
			//pixColor -= 0.6;

			//draw the pixel
			glBegin(GL_POINTS);
			glColor3f(1- 0.8 * pixColor,1-0.6 * pixColor, 1-0.2 *pixColor);
			glVertex2i(x, y);
			glEnd();
		}
	}
	glEnable(GL_DEPTH);
}

void display()
{
	glClearColor(0.2, 0.6, 0.8, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

	///*Draw Clouds comment out if need be*/
	//drawClouds();

	////drawTerrain();
 //   drawTerrain3D();

	//drawTrees();

	drawCactus();
	//flush all changes
	glutSwapBuffers();
	glFlush();
}

/*redraw display*/
//void idle()
//{
//	/* Redraw the window */
//	glutPostRedisplay();
//}

void plantTrees(int factor)
{
	for (int z = 0; z < 1024; z++)
	{
		for (int x = 0; x < 1024; x++)
		{
			if (rand() % factor == 0)
			{
				if (t->terrain[z][x] /60 - 10 < 150) 
				{
					genTree(x / 20 - 20, t->terrain[z][x] / 60 - 10, z);
				}
			}
		}
	}
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
	//genTree(0.2,0.0,20.0);

	//genTree(0.2, 0.0, 25.0);

	//generate a tree
	//genTree(0.5, 0.0, 20.9);

	//genTree(1, 0.0, 21.0);

	//generate a cactus
	genCactus(0.2,0.0,22);

    /* Initialize the GLUT window */
    glutInit(&argc, argv);
	glutInitWindowSize(winWidth, winHeight);
    glutInitWindowPosition(30, 30);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutCreateWindow("CS 334 - Procedural Modeling");

	srand(time(NULL));
	t = new Terrain(rightEndpointX, generatingMountains);

	srand(time(NULL));
	generatingMountains = false;
	t = new Terrain(rightEndpointX, generatingMountains);
	t->generateEndpoints3D();
	generatingDesert = false;
	int treeFactor = 100;
	if (generatingDesert)
	{
		t->TerrainGenerate(1024, .07);
	}
	else if (generatingMountains)
	{
		t->TerrainGenerate(1024, .1);
		plantTrees(100);
	}
	else
	{
		t->TerrainGenerate(1024, .05);

		plantTrees(100);
	}

	//need something to start with what kind of terrain we're generating
//	t->generateEndpoints();
	//calculate values, store in array
//	t->calcMidpoints(leftEndpointX, t->heights[leftEndpointX], rightEndpointX, t->heights[rightEndpointX], roughnessFactor);
//	t->makePicture();


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
