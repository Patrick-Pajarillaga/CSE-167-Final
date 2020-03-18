#include "ParticleController.h"

ParticleController::ParticleController()
{
	lastTime = glfwGetTime();
	//Set Color
	color = glm::vec3(1.0f, 0.95f, 0.1f);
	model = glm::mat4(1);

	//Make the vertices of the quad
	vertices.push_back(glm::vec3(-0.5f, -0.5f, 0));
	vertices.push_back(glm::vec3(0.5, -0.5f, 0));
	vertices.push_back(glm::vec3(-0.5, 0.5, 0));
	vertices.push_back(glm::vec3(0.5, 0.5, 0));

	for (int i = 0; i < maxParticles; i++) {
		//srand(5000);
		position.push_back(glm::vec4(0, 5, i, 1));
		partColor.push_back(glm::vec4(0, 0.467f, 0.745f, 0.5));
		container[i].pos = glm::vec3(0, 5, (GLfloat)0);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);
		container[i].speed = glm::vec3(0, 10, 0) + randomdir * 3.0f;
		container[i].life = -1.0f;
	}
	
	
	// Generate a vertex array (VAO) and one vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbo);
	glBindVertexArray(vao);

	//PASSING IN QUAD VERTICES
	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glVertexAttribDivisor(0, 0);

	//PASSING IN POSITION
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * position.size(),
		position.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribDivisor(1, 1);

	//PASSING IN COLOR
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * partColor.size(),
		partColor.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glVertexAttribDivisor(2, 1);
	

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

ParticleController::~ParticleController()
{
	glDeleteBuffers(3, vbo);
	glDeleteVertexArrays(1, &vao);
}

void ParticleController::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);

	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	//glPointSize(100.0);
	glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, maxParticles);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void ParticleController::update()
{
	double currentTime = glfwGetTime();
	double delta = currentTime - lastTime;
	lastTime = currentTime;

	int newParticles = (int)(delta * 1000.0);
	if (newParticles > (int)(0.016f * 1000.0)) {
		newParticles = (int)(0.016f * 1000.0);
	}
	for (int k = 0; k < newParticles; k++) {
		int particleIndex = FindUnusedParticle();
		if (counter == 25) {
			counter = 0;
		}
		container[particleIndex].pos = glm::vec3(0, 100, counter);
		glm::vec3 randomdir = glm::vec3(
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f,
			(rand() % 2000 - 1000.0f) / 1000.0f
		);
		container[particleIndex].speed = glm::vec3(0, 1, 0) + randomdir * 3.0f;
		container[particleIndex].life = 6.0f;
		partColor[particleIndex] = (glm::vec4(0, 0.467f, 0.745f, 0.5));
		counter++;
	}

	
	for (int i = 0; i < position.size(); i++) {
		/*
		if (container[i].life < 0.0) {
			container[i].pos = glm::vec3((GLfloat)i / 1000, 0, (GLfloat)0);
			glm::vec3 randomdir = glm::vec3(
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f,
				(rand() % 2000 - 1000.0f) / 1000.0f
			);
			container[i].speed = glm::vec3(0, 10, 0) + randomdir * 1.5f;
			container[i].life = 5.0f;
		}
		*/
		container[i].life -= (float)delta;
		container[i].speed += glm::vec3(0.0f, -9.81f, 0.0f) * (float)delta * 0.5f;
		container[i].pos += container[i].speed * (float)delta;
		position[i] = glm::vec4(container[i].pos, 1);

	}
	//color = vec3(0.5 , k , k);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * position.size(), position.data(), GL_STATIC_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * position.size(), position.data());

	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * partColor.size(), partColor.data(), GL_STATIC_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(glm::vec4) * partColor.size(), partColor.data());
	
}

int ParticleController::FindUnusedParticle()
{
	for (int i = LastUsedParticle; i < maxParticles; i++) {
		if (container[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	for (int i = 0; i < LastUsedParticle; i++) {
		if (container[i].life < 0) {
			LastUsedParticle = i;
			return i;
		}
	}

	return 0; // All particles are taken, override the first one
}

void ParticleController::sortParticles()
{

}
