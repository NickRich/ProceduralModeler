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
}

Tree Tree::genTree(Tree * t)
{
	//
	if (t->trunk == 0)
	{

	}

	//add each tree segment to a list of braches
	branches.push_back(t);


	return *t;
	
}

Tree::~Tree()
{
}
