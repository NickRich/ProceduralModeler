#include "Cactus.h"
#include <stdio.h>
#include <stdlib.h>
#include "GL/glew.h"
#include "GL/glut.h"
#include <vector>
using namespace std;


Cactus::Cactus(float x, float y, float z)
{
	//postion
	this->x = x;
	this->y = y;
	this->z = z;
	//angles
	angleX = 90.0;
	angleY = 0.0;
	angleZ = 0.0;
	height = 1.5;
	//radius
	radiusTop = 0.1;
	radiusBottom = 0.1;
	//if its the trunk
	trunk = 1;
	//if 1 then rightside, 0 then left side
	rl = 0;
}

Cactus * Cactus::genBranches(Cactus * c)
{
	//if trunk, make a branch 
	if (c->trunk == 1)
	{
		//add each tree segment to a list of braches
		branches.push_back(c);
		c->trunk = 0;
		genBranches(c);
		return c;
	}

	float bx = 0.0;
	float by = 0.0;
	float bz = 0.0;

	Cactus * leftBranch1;
	Cactus * leftBranch2;
	Cactus * rightBranch1;
	Cactus * rightBranch2;

	//setup left side with 2 branches

		//set up the left branch

		//create a branch shifted left and on top of the previous
		bx = c->x - sin(c->angleY)*c->height / 2;
		by = c->y + c->height/1.7;
		bz = c->z;

		leftBranch1 = new Cactus(bx, by, bz);
		leftBranch1->trunk = 0;
		leftBranch1->angleY = -90;

		leftBranch1->height = c->height - c->height*0.6;
		leftBranch1->radiusTop = c->radiusTop;// -0.4 * c->radiusTop;
		leftBranch1->radiusBottom = c->radiusBottom;// -0.4*c->radiusBottom;
		branches.push_back(leftBranch1);


		////set up right branch

		bx = c->x - sin(c->angleY)*c->height / 2 - leftBranch1->height;
		by = c->y + c->height/1.7;
		bz = c->z;

		leftBranch2 = new Cactus(bx, by, bz);
		leftBranch2->trunk = 0;
		leftBranch2->angleY = 0.0;
		leftBranch2->height = c->height  - c->height*0.6;
		leftBranch2->radiusTop = leftBranch1->radiusTop;
		leftBranch2->radiusBottom = leftBranch1->radiusBottom;
		branches.push_back(leftBranch2);
		//change sides
		rl = 1;

	
		//set up the left branch
		////create a branch shifted left and on top of the previous
		bx = c->x + sin(c->angleY)*c->height / 2;
		by = c->y + c->height / 1.5;
		bz = c->z;

		rightBranch1 = new Cactus(bx, by, bz);
		rightBranch1->trunk = 0;
		rightBranch1->angleY = 90;

		rightBranch1->height = c->height - c->height*0.6;
		rightBranch1->radiusTop = c->radiusTop;// -0.4 * c->radiusTop;
		rightBranch1->radiusBottom = c->radiusBottom;// -0.4*c->radiusBottom;
		branches.push_back(rightBranch1);

		//set up right branch
		bx = c->x + sin(c->angleY)*c->height / 2 + rightBranch1->height;
		by = c->y + c->height / 1.5;
		bz = c->z;

		rightBranch2 = new Cactus(bx, by, bz);
		rightBranch2->trunk = 0;
		rightBranch2->angleY = 0.0;
		rightBranch2->height = c->height - c->height*0.6;
		rightBranch2->radiusTop = rightBranch1->radiusTop;
		rightBranch2->radiusBottom = rightBranch1->radiusBottom;
		branches.push_back(rightBranch2);
		//change sides
		rl = 0;


	

	if (leftBranch1->height <= 0.0 || rightBranch1->height <= 0.2)
	{
		return c;
	}

	//recurse on each branch
	genBranches(leftBranch2);
	genBranches(rightBranch2);

}

Cactus::~Cactus()
{
}
