#pragma once
#include <vector>
using namespace std;
class Tree
{
public:
	Tree(float x, float y, float z);
	Tree* genBranches(Tree * t);
	~Tree();
	float height;
	float radiusTop;
	float radiusBottom;
	float angleX;
	float angleY;
	float angleZ;
	float x;
	float y;
	float z;
	vector <Tree *> branches;
	int trunk;
	int rl;

};

