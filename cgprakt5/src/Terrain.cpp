#include "Terrain.h"
#include "rgbimage.h"
#include "Terrainshader.h"
#define ASSET_DIRECTORY "../../assets/"

Terrain::Terrain(const char* HeightMap, const char* DetailMap1, const char* DetailMap2) : Size(10, 1, 10)
{
	if (HeightMap && DetailMap1 && DetailMap2)
	{
		bool loaded = load(HeightMap, DetailMap1, DetailMap2, ASSET_DIRECTORY "mixmap.bmp");
		if (!loaded)
			throw std::exception();

	}
}

Terrain::~Terrain()
{

}


bool Terrain::load(const char* HeightMap, const char* DetailMap1, const char* DetailMap2, const char* MixTex)
{
	if (!HeightTex.load(HeightMap))
		return false;
	if (!DetailTex[0].load(DetailMap1))
		return false;
	if (!DetailTex[1].load(DetailMap2))
		return false;
	if (!this->MixTex.load(MixTex))
		return false;

	const RGBImage* img = HeightTex.getRGBImage();
	float height = img->height();
	float width = img->width();

	/*
	//aufgabe2
	RGBImage dst = RGBImage(height, width);
	RGBImage::SobelFilter(dst, *img, 10.0f);
	dst.saveToDisk("mixmap.bmp");
	*/

	std::vector<Vector> normals;
	std::vector<Vector> vertices;


	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {

			normals.push_back(Vector(0, 0, 0));
			float vertexX = ((y / width) - 0.5f) * Size.X;
			float vertexY = img->getPixelColor(x, y).R;
			float vertexZ = ((x / height) - 0.5f) * Size.Z;
			Vector vertex = Vector(vertexX, vertexY, vertexZ);
			vertices.push_back(vertex);
		}
	}

	IB.begin();
	VB.begin();

	for (int x = 0; x < height - 1; x++) {
		for (int y = 0; y < width - 1; y++) {
			// 1. Dreieck
			int temp = y + x * width;
			unsigned int a = temp + 1;
			unsigned int b = temp;
			unsigned int c = temp + width;

			IB.addIndex(a);
			IB.addIndex(b);
			IB.addIndex(c);

			Vector v1(vertices[a]), v2(vertices[b]), v3(vertices[c]);

			Vector normaleDreieck01 = (v2 - v1).cross(v3 - v1).normalize();

			normals[a] += normaleDreieck01;
			normals[b] += normaleDreieck01;
			normals[c] += normaleDreieck01;

			// 2. Dreieck
			a = temp + width;
			b = temp + width + 1;
			c = temp + 1;

			IB.addIndex(a);
			IB.addIndex(b);
			IB.addIndex(c);

			v1 = vertices[a];
			v2 = vertices[b];
			v3 = vertices[c];

			Vector normaleDreieck02 = (v2 - v1).cross(v3 - v1).normalize();
			normals[a] += normaleDreieck02;
			normals[b] += normaleDreieck02;
			normals[c] += normaleDreieck02;
		}
	}

	for (int x = 0; x < height; x++)
	{
		for (int y = 0; y < width; y++)
		{
			VB.addTexcoord0(0, 0);
			VB.addNormal(normals[y + x * width]);
			VB.addVertex(vertices[y + x * width]);
		}
	}
	IB.end();
	VB.end();

	return true;

	/*

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
		//	VB.addNormal(calcNormal2(i , j , img));
			VB.addTexcoord0(0.0f, 0.0f);
			VB.addNormal(normalen[j + i * width]);
		//	Vector vertex = Vector(vertices[i + 1][j + 1]);
			Vector vertex = vertices[j + i * width];
			VB.addVertex(Vector(vertex));

		}
	}

	for (int i = 0; i < height - 1; i++) {
		for (int j = 0; j < width - 1; j++) {
			int temp = j + i * width;
			IB.addIndex(temp);
			IB.addIndex(temp + 1);
			IB.addIndex(temp + 1 + width);
			IB.addIndex(temp + 1 + width);
			IB.addIndex(temp + width);
			IB.addIndex(temp);
		}
	}


	for (int z = 0; z < height; z++) {
		for (int x = 0; x < width; x++) {
			int topLeft = (z * width) + x;
			int topRight = topLeft + 1;
			int bottomLeft = ((z + 1) * width) + x;
			int bottomRight = bottomLeft + 1;
			IB.addIndex(topLeft);
			IB.addIndex(bottomLeft);
			IB.addIndex(topRight);
			IB.addIndex(topRight);
			IB.addIndex(bottomLeft);
			IB.addIndex(bottomRight);

		}

	}


}



/*
Vector Terrain::calcNormal2(int x, int y, Vector** vertices) {

	Vector normal;

	normal += this->normal(vertices[x][y], vertices[x - 1][y], vertices[x - 1][y - 1]);

	normal += this->normal(vertices[x][y], vertices[x - 1][y - 1], vertices[x][y - 1]);

	normal += this->normal(vertices[x][y], vertices[x][y - 1], vertices[x + 1][y]);

	normal += this->normal(vertices[x][y], vertices[x + 1][y], vertices[x + 1][y + 1]);

	normal += this->normal(vertices[x][y], vertices[x + 1][y + 1], vertices[x][y + 1]);

	normal += this->normal(vertices[x][y], vertices[x][y + 1], vertices[x - 1][y]);

	return normal.normalize();

}

Vector Terrain::normal(const Vector& a, const Vector& b, const Vector& c) {
	Vector triangleNormal = (b - a).cross(c - a);
	return triangleNormal.normalize();
}

*/
}
Vector Terrain::calcNormal2(int x, int y, const RGBImage* image) {
	/*
	float heightL = getHeight(x - 1, y, image);
	float heightR = getHeight(x + 1, y, image);
	float heightD = getHeight(x, y - 1, image);
	float heightU = getHeight(x, y + 1, image);
	*/

	float heightL = image->getPixelColor(x - 1, y).R;
	float heightR = image->getPixelColor(x +1, y).R;
	float heightD = image->getPixelColor(x, y - 1).R;
	float heightU = image->getPixelColor(x, y + 1).R;

	Vector normale = Vector(heightL - heightR, 2.0f, heightD - heightU);
	return normale.normalize();
}


float Terrain::getHeight(int x, int y, const RGBImage* image) {
	if (x < 0 || x >= image->height() || y < 0 || y >= image->width()) {
		return 0;
	}
	Color col = image->getPixelColor(x, y);
	float height = col.R;


	height += image->height() / 2.0f;
	height /= image->height() / 2.0f;
	height *= image->height();

	return height;
}



Vector Terrain::getVector(int x, int y, const RGBImage* image) {
	Color col = image->getPixelColor(x, y);
	float pixelY = (col.R + col.G + col.B) / 3;

	float pixelX = ((x / image->width()) - 0.5f) * Size.X;
	float pixelZ = ((y / image->height()) - 0.5f) * Size.Z;

	return Vector(pixelX, pixelY, pixelZ);
	/*
float xxx = ((x / image->width()) - 0.5f) * Size.X;
Color col = image->getPixelColor(y, x);
float yyy = (col.R + col.G + col.B) / 3;
float zzz = ((y / image->height()) - 0.5f) * Size.Z;
return Vector(xxx / 10, yyy / 10, zzz / 10);
*/
}

void Terrain::shader(BaseShader* shader, bool deleteOnDestruction)
{
	BaseModel::shader(shader, deleteOnDestruction);
}

void Terrain::draw(const BaseCamera& Cam)
{
	applyShaderParameter();
	BaseModel::draw(Cam);

	VB.activate();
	IB.activate();
	glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
	IB.deactivate();
	VB.deactivate();
}

void Terrain::applyShaderParameter()
{
	TerrainShader* Shader = dynamic_cast<TerrainShader*>(BaseModel::shader());
	if (!Shader)
		return;

	Shader->mixTex(&MixTex);
	for (int i = 0; i < 2; i++)
		Shader->detailTex(i, &DetailTex[i]);
	Shader->scaling(Size);

	// TODO: add additional parameters if needed..
}
