#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"
#include <string>

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
	GLuint cubemapTexture;
	int noiseHeight, noiseWidth;
	double noi[128][128];
	//unsigned char* data;
	unsigned char boomer[512][512][3];
public:
	Cube(float size);
	~Cube();

	void draw();
	void update();

	void spin(float deg);
	GLuint loadCubemap(std::vector<std::string> faces);
	GLuint updateCubemap(std::vector<std::string> faces);
	unsigned char noise(int x, int y, int random);
	float fade(float t);
	void genNoise();
	double smoothNoise(double x, double y);
	double turbulence(double x, double y, double size);
};

#endif

