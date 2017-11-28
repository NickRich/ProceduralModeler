#pragma once
#include <vector>
using namespace std;
class Tree
{
public:
	Tree(float x, float y, float z);
	Tree genTree(Tree * t);
	~Tree();
	float height;
	float radiusTop;
	float radiusBottom;
	float angle;
	float x;
	float y;
	float z;
	vector <Tree *> branches;
	int trunk;
	
};

