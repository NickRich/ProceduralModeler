#pragma once
class Terrain {

public:
	Terrain(int winWidth, float s, bool mountains);
	float heights[500];
	float roughnessFactor;
	bool genMountains;
	int width;
	float pixels[500][500];

	float generateRandomOffset(float aX, float aY, float bX, float bY, float s);
	float generateMidpoint(float a, float b, float r);
	void calcMidpoints(int leftX, int leftY, int rightX, int rightY);
	void generateEndpoints();
	void makePicture();
};