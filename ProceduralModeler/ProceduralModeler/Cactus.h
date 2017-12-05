#pragma once
#include <vector>
using namespace std;
class Cactus
{
public:
	Cactus(float x, float y, float z);
	Cactus * genBranches(Cactus*);
	~Cactus();
	float height;
	float radiusTop;
	float radiusBottom;
	float angleX;
	float angleY;
	float angleZ;
	float x;
	float y;
	float z;
	vector <Cactus *> branches;
	int trunk;
	int rl;
};

