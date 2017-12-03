#include "Tree.h"
#include "Cloud.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
using namespace std;

Tree::Tree(float x, float y, float z)
{
	//postion
	this->x = x;
	this->y = y;
	this->z = z;
	//angles
	angleX = 90.0;
	angleY = 0.0;
	angleZ = 0.0;
	height = 0.5;
	//radius
	radiusTop = 0.06;
	radiusBottom = 0.06;
	//if its the trunk
	trunk = 1;
	//horizontal branch shift
	xShift = 0.0;
	//if 1 then rightside, 0 then left side
	rl = 0;
	//width of cylinder used to shift over left side branches
	leftWidth = 0;
}

Tree *Tree::genBranches(Tree * t)
{
	//if trunk, make a branch 
	//modify branch to be smaller and thinner
	//change position anlge to be random;
	if (t->trunk == 1)
	{
		//add each tree segment to a list of braches
		branches.push_back(t);
		t->trunk = 0;
		genBranches(t);
		return t;
	}
	
	float bx = 0.0;
	float by = 0.0;
	float bz = 0.0;
	
	Tree * leftBranch;
	Tree * rightBranch;


	//setup left side with 2 branches
	if (rl == 0)
	{
		//set up the left branch
		
		//create a branch shifted left and on top of the previous
		bx = t->x - sin(t->angleY)*t->height / 2;
		by = t->y + t->height -0.2*t->height;
		bz = t->z;// 0.5 * ((rand() % 32768) / 32768.0);

		leftBranch = new Tree(bx, by, bz);
		leftBranch->trunk = 0;
		leftBranch->angleY = -30;
		//leftBranch->angleY = (rand() % 20 + 20);
		//leftBranch->x -= t->x - sin(leftBranch->angleY)*t->height / 2;
		leftBranch->height = t->height - 0.1;
		leftBranch->radiusTop = t->radiusTop - 0.4 * t->radiusTop;
		leftBranch->radiusBottom = t->radiusBottom - 0.3*t->radiusBottom;
		branches.push_back(leftBranch);

		//set up right branch
	
		bx = t->x + sin(t->angleY)*t->height / 2;
		by = t->y + t->height -0.2*t->height;
		bz = t->z;	

		rightBranch = new Tree(bx, by, bz);
		rightBranch->trunk = 0;
		rightBranch->angleY = -leftBranch->angleY;// 30;
		//rightBranch->x -= t->x + sin(rightBranch->angleY)*t->height / 2;
		rightBranch->height = t->height - 0.1;
		rightBranch->radiusTop = leftBranch->radiusTop;
		rightBranch->radiusBottom = leftBranch->radiusBottom;
		branches.push_back(rightBranch);
		//change sides
		rl = 1;
		
	}
	//setup right side with 2 branches
	else
	{
		//set up the left branch
		//create a branch shifted left and on top of the previous
		bx = t->x - sin(t->angleY)*t->height / 2;
		by = t->y + t->height -0.2*t->height;
		bz = t->z;// +((rand() - 32768) / 32768.0);
		
		leftBranch = new Tree(bx, by, bz);
		leftBranch->trunk = 0;
		leftBranch->angleY = -30;
		//leftBranch->angleY = (rand() % 20 + 20);
		//leftBranch->x -= t->x - sin(leftBranch->angleY)*t->height / 2;
		leftBranch->angleZ = 60;
		leftBranch->height = t->height - 0.1;
		leftBranch->radiusTop = t->radiusTop - 0.4 * t->radiusTop;
		leftBranch->radiusBottom = t->radiusBottom - 0.3*t->radiusBottom;
		branches.push_back(leftBranch);

		//set up right branch
		bx = t->x + sin(t->angleY)*t->height / 2;
		by = t->y + t->height -0.2*t->height;		
		bz = t->z;
		rightBranch = new Tree(bx, by, bz);
		rightBranch->trunk = 0;
		rightBranch->angleY = -leftBranch->angleY;//30;
		//rightBranch->x -= t->x + sin(rightBranch->angleY)*t->height / 2;
		rightBranch->height = t->height - 0.1;
		rightBranch->radiusTop = leftBranch->radiusTop;
		rightBranch->radiusBottom = leftBranch->radiusBottom;
		branches.push_back(rightBranch);
		//change sides
		rl = 0;
	

	}
	
	if (leftBranch->height <= 0.0 || rightBranch->height <= 0.0)
	{
		return t;
	}

	//recurse on each branch
	genBranches(leftBranch);
	genBranches(rightBranch);
	
	

	
}

Tree::~Tree()
{
}
