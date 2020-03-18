#include "Cube.h"
#include "stb_image.h"
#include <iostream>
#include <chrono>
#include <thread>

Cube::Cube(float size)
{
	// Model matrix. Since the original size of the cube is 2, in order to
	// have a cube of some size, we need to scale the cube by size / 2.
	//model = glm::scale(glm::vec3(size / 2.f));
	noiseHeight = noiseWidth = 128;
	model = glm::scale(glm::vec3(128, 1, 128));

	// The color of the cube. Try setting it to something else!
	color = glm::vec3(0.5f, 0.95f, 0.5f);

	/*
	 * Cube indices used below.
	 *    4----7
	 *   /|   /|
	 *  0-+--3 |
	 *  | 5--+-6
	 *  |/   |/
	 *  1----2
	 *
	 */

	 // The 8 vertices of a cube.
	std::vector<glm::vec3> vertices
	{
		glm::vec3(-1, 1, 1),
		glm::vec3(-1, -1, 1),
		glm::vec3(1, -1, 1),
		glm::vec3(1, 1, 1),
		glm::vec3(-1, 1, -1),
		glm::vec3(-1, -1, -1),
		glm::vec3(1, -1, -1),
		glm::vec3(1, 1, -1)
	};

	// Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2 
	// and v3 in counter-clockwise order.
	std::vector<glm::ivec3> indices
	{
		// Front face.
		/*
		glm::ivec3(0, 1, 2),
		glm::ivec3(2, 3, 0),
		
		// Back face.
		glm::ivec3(7, 6, 5),
		glm::ivec3(5, 4, 7),
		
		// Right face.
		glm::ivec3(3, 2, 6),
		glm::ivec3(6, 7, 3),
		// Left face.
		glm::ivec3(4, 5, 1),
		glm::ivec3(1, 0, 4),
		*/
		// Top face.
		glm::ivec3(4, 0, 3),
		glm::ivec3(3, 7, 4),
		/*
		// Bottom face.
		glm::ivec3(1, 5, 6),
		glm::ivec3(6, 2, 1),
		*/
	};
	

	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);
	glGenBuffers(2, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(),
		vertices.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0. 
	// We will be able to access vertices through it.
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	// Bind to the second VBO. We will use it to store the indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);

	std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	//glEnable(GL_TEXTURE_CUBE_MAP);
	genNoise();
	cubemapTexture = loadCubemap(faces);
	
}

Cube::~Cube()
{
	// Delete the VBOs and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Cube::draw()
{
	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// elemnt array buffer.
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Cube::update()
{
	//std::this_thread::sleep_for(std::chrono::milliseconds(500));
	// Spin the cube by 1 degree.
	std::vector<std::string> faces
	{
			"right.jpg",
			"left.jpg",
			"top.jpg",
			"bottom.jpg",
			"front.jpg",
			"back.jpg"
	};
	//genNoise();
	cubemapTexture = updateCubemap(faces);
	//draw();
	//spin(0.001f);
}

void Cube::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(glm::mat4(1), glm::radians(deg), 
		glm::vec3(0.0f, 1.0f, 0.0f)) * model;
}

GLuint Cube::loadCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	//printf("%d", faces.size());
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		//unsigned char* data; //= stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		//data = new unsigned char[16][16*3];
		//unsigned char boomer[16][16][3];
		unsigned char temp;
		int x, y;
		x = 0;
		y = 0;
		//unsigned char* gamer = new unsigned char[32 * 32];
		for (int k = 0; k < 512; k++) {
			for (int j = 0; j < 512; j++) {
				temp = (unsigned char)turbulence(k, j, 64);
				if (temp < 5) {
					boomer[k][j][0] = 255;
					boomer[k][j][1] = 255;
					boomer[k][j][2] = 255;
				}
				else {
					boomer[k][j][0] = temp;
					boomer[k][j][1] = temp;
					boomer[k][j][2] = temp;
				}
			}
		}
		
		width = 512;
		height = 512;
		if (boomer)
		{
		
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, boomer
			);
			//stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(boomer);
		}
	}
	//glGenerateMipmap(textureID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	return textureID;
}

GLuint Cube::updateCubemap(std::vector<std::string> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	
	unsigned char temp, zerotemp;
	int width, height;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		for (int k = 0; k < 512; k++) {
			for (int j = 0; j < 512-1; j++) {
				if (j == 0) {
					zerotemp = boomer[k][j][0];
				}
				boomer[k][j][0] = boomer[k][j+1][0];
				boomer[k][j][1] = boomer[k][j+1][1];
				boomer[k][j][2] = boomer[k][j+1][2];
				
			}
			boomer[k][511][0] = zerotemp;
			boomer[k][511][1] = zerotemp;
			boomer[k][511][2] = zerotemp;
		}

		width = 512;
		height = 512;
		if (boomer)
		{
			//printf("%u\n", data);
			//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, boomer
			);
			//stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << std::endl;
			stbi_image_free(boomer);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

	return textureID;
}

unsigned char Cube::noise(int x, int y, int random)
{
	return (unsigned char)rand() % 255;

}

float Cube::fade(float t)
{
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
	
}

void Cube::genNoise()
{
	for (int y = 0; y < 128; y++) {
		for (int x = 0; x < 128; x++)
		{
			noi[y][x] = (rand() % 32768) / 32768.0;
			//printf("At %d, %d have %f\n", x, y, noi[y][x]);
		}
	}
}

double Cube::smoothNoise(double x, double y)
{
	//printf("%f, %f\n", x, y);
	//get fractional part of x and y
	double fractX = x - int(x);
	double fractY = y - int(y);
	//printf("%f, %f\n", fractX, fractY);

	//wrap around
	int x1 = (int(x) + 128) % 128;
	int y1 = (int(y) + 128) % 128;
	//printf("%d, %d\n", x1, y1);

	//neighbor values
	int x2 = (x1 + noiseWidth - 1) % noiseWidth;
	int y2 = (y1 + noiseHeight - 1) % noiseHeight;

	//smooth the noise with bilinear interpolation
	double value = 0.0;
	value += fractX * fractY * noi[y1][x1];
	//("%f\n", noi[50][50]);
	value += (1 - fractX) * fractY * noi[y1][x2];
	//printf("%f\n", value);
	value += fractX * (1 - fractY) * noi[y2][x1];
	//printf("%f\n", value);
	value += (1 - fractX) * (1 - fractY) * noi[y2][x2];
	//printf("%f\n", value);

	return value;
}

double Cube::turbulence(double x, double y, double size)
{
	double value = 0.0, initialSize = size;

	while (size >= 1)
	{
		value += smoothNoise(x / size, y / size) * size;
		size /= 2.0;
	}

	return(128.0 * value / initialSize);
}



