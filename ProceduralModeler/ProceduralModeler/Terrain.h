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

	float terrain[1025][1025];
	void generateEndpoints3D();
	float Terrain::Offset3D(int aX, int aY, int aZ, int bX, int bY, int bZ, float s);
	void TerrainGenerate(int range);
	void printHeights();
};