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
	angle = 80.0;
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
	Tree * branch;
	if (rl == 0)
	{
		
		bx = t->x + 4*t->radiusTop;
		by = t->y;
		float bz = t->z;
		branch = new Tree(bx, by, bz);
		branch->trunk = 0;
		//branch->angle = rand() % 20 + 60;
		branch->height = t->height;
		branch->radiusTop = t->radiusTop;
		branch->radiusBottom = t->radiusBottom;
		branch->angle = rand() % 20 + 60;
	}
	else
	{
		bx = t->x - t->radiusTop;
		by = t->y + 2*t->height;
		float bz = t->z;// 0.5 * ((rand() % 32768) / 32768.0);
		branch = new Tree(bx, by, bz);
		branch->trunk = 0;
		//branch->angle = rand() % 20 + 60;
		branch->height = t->height - 0.1;
		branch->radiusTop -= 0.02;
		branch->radiusBottom -= 0.01;
	
	}
	
	

	if (rl == 1)
	{
		rl = 0;
	}
	else
	{
		rl = 1;
	}
	branches.push_back(branch);
	
	if (branch->height <= 0.4)
	{
		return t;
	}
	genBranches(branch);

	


	
}

Tree::~Tree()
{
}
