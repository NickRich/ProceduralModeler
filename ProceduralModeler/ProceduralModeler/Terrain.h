#pragma once
class Terrain {

public:
	Terrain(int winWidth, bool mountains);
	float heights[500];
	bool genMountains;
	int width;
	float pixels[500][500];

	float generateRandomOffset(float aX, float aY, float bX, float bY, float s);
	float generateMidpoint(float a, float b, float r);
	void calcMidpoints(int leftX, int leftY, int rightX, int rightY, float s);
	void generateEndpoints();
	void makePicture();
};