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
	this->x = x;
	this->y = y;
	this->z = z;
	angleX = 90.0;
	angleY = 0.0;
	angleZ = 0.0;
	height = 0.5;
	radiusTop = 0.06;
	radiusBottom = 0.06;
	trunk = 1;
	rl = 1;
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
	
	//if rl is one make a right branch, otherwise make a left
	float bx = 0.0;
	float by = 0.0;
	float bz = 0.0;
	//}
	//make a left side branch
	//else
	//{
		Tree * leftBranch;
		//create a branch shifted left and on top of the previous
		bx = t->x - t->radiusTop;
		by = t->y + t->height - 0.1;
		bz = t->z;// 0.5 * ((rand() % 32768) / 32768.0);
		leftBranch = new Tree(bx, by, bz);
		leftBranch->trunk = 0;
		//leftBranch->angleY = 50;
		leftBranch->height = t->height - 0.1;
		leftBranch->radiusTop -= 0.02;
		leftBranch->radiusBottom -= 0.01;
		branches.push_back(leftBranch);

	//}
		
		Tree * rightBranch;
		//make a right side branch
		/*if (t->rl == 0)
		{*/
		//create a branch shifted right and on top of the previous
		bx = t->x + t->radiusTop;
		by = t->y + t->height - 0.1;;
		bz = t->z;
		rightBranch = new Tree(bx, by, bz);
		rightBranch->trunk = 0;
		//rightBranch->angleY = -50;
		rightBranch->height = t->height - 0.1;
		rightBranch->radiusTop = leftBranch->radiusTop;
		rightBranch->radiusBottom = leftBranch->radiusBottom;
		branches.push_back(rightBranch);

	
	//make the other branch side next time
	/*if (t->rl == 1)
	{
		t->rl = 0;
	}
	else
	{
		t->rl = 1;
	}
	branches.push_back(branch);*/
	
	if (leftBranch->height <= 0.1 || rightBranch->height <= 0.1)
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
