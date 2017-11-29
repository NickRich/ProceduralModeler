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

	float terrain[513][513];
	void generateEndpoints3D();
	void squareDivisionUp(int rightX, int rightZ, int downX, int downZ, int leftX, int leftZ);
	void squareDivisionRight(int downX, int downZ, int leftX, int leftZ, int upX, int upZ);
	void squareDivisionDown(int leftX, int leftZ, int upX, int upZ, int rightX, int rightZ);
	void squareDivisionLeft(int upX, int upZ, int rightX, int rightZ, int downX, int downZ);

	void diamondDivision(int bottomLeftX, int bottomLeftZ, int bottomRightX, int bottomRightZ, int topLeftX, int topLeftZ, int topRightX, int topRightZ);
	void printHeights();
};