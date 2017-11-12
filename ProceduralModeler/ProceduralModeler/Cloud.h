#pragma once
class Cloud
{
public:
	Cloud();
	void setValues(int xPos,int yPos, float darkness, int width, int length);
	float ** genNoise();
	~Cloud();
	int xPos;
	int yPos;
	float darkness;
	int width;
	int length;

};