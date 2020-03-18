#pragma once
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <algorithm>

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <random>
using namespace glm;

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r, g, b, a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if <0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

class ParticleController
{
private:
	GLuint vao, vbo[3];
	double lastTime;
	const int maxParticles = 1000;
	int counter = 0;
	int LastUsedParticle = 0;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec4> position;
	std::vector<glm::vec4> partColor;
	glm::vec3 color;
	glm::mat4 model;
	Particle container[1000];
public:
	ParticleController();
	~ParticleController();
	void draw();
	void update();
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	int FindUnusedParticle();
	void sortParticles();
};

