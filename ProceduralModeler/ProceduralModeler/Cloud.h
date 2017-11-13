#pragma once
#include <vector>
using namespace std;
class Cloud
{
public:
	Cloud();
	void setValues(int xPos,int yPos, float darkness, int width, int length);
	void genNoise();
	float smoothNoise(float x, float y);
	~Cloud();
	int xPos;
	int yPos;
	float darkness;
	int width;
	int length;
		
};