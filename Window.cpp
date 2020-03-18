#include "Window.h"

/* 
 * Declare your variables below. Unnamed namespace is used here to avoid 
 * declaring global or static variables.
 */
namespace
{
	int width, height;
	std::string windowTitle("GLFW Starter Project");

	Cube* cube;
	Object* currentObj; // The object currently displaying.
	ParticleController* tester;
	int counter = 0;

	glm::vec3 eye(0, -5, 20); // Camera position.
	glm::vec3 center(0, 0, 0); // The point we are looking at.
	glm::vec3 up(0, 1, 0); // The up direction of the camera.
	glm::vec3 camDir = glm::normalize(eye - center);
	glm::vec3 camRight = glm::normalize(glm::cross(up, camDir));
	glm::vec3 camUp = glm::cross(camDir, camRight);
	float fovy = 60;
	float near = 1;
	float far = 1000;
	glm::mat4 view = glm::lookAt(eye, center, up); // View matrix, defined by eye, center and up.
	glm::mat4 projection; // Projection matrix.

	GLuint program; // The shader program id.
	GLuint skybox;
	GLuint projectionLoc; // Location of projection in shader.
	GLuint viewLoc; // Location of view in shader.
	GLuint modelLoc; // Location of model in shader.
	GLuint colorLoc; // Location of color in shader.

};

bool Window::initializeProgram()
{
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	skybox = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");

	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
	cube = new Cube(10000.0f);
	tester = new ParticleController();

	// Set cube to be the first to display
	currentObj = cube;

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete cube;
	delete tester;

	// Delete the shader program.
	glDeleteProgram(program);
	glDeleteProgram(skybox);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle.c_str(), NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int w, int h)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height);
#endif
	width = w;
	height = h;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	projection = glm::perspective(glm::radians(fovy),
		(float)width / (float)height, near, far);
}

void Window::idleCallback()
{
	// Perform any updates as necessary. 
	if (counter % 100 == 0) {
		//tester->update();
		currentObj->update();
	}
	//currentObj->update();
	counter++;
	tester->update();
}

void Window::displayCallback(GLFWwindow* window)
{
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(skybox);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glDepthMask(GL_FALSE);
	glm::mat4 model = currentObj->getModel();

	glUniformMatrix4fv(glGetUniformLocation(skybox, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(skybox, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(skybox, "model"), 1, GL_FALSE, glm::value_ptr(model));

	currentObj->draw();
	glDepthMask(GL_TRUE);

	glUseProgram(program);
	glDisable(GL_CULL_FACE);
	model = tester->getModel();
	glm::vec3 color = tester->getColor();
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));
	glUniform3fv(glGetUniformLocation(program, "cameraRight"), 1, glm::value_ptr(camRight));
	glUniform3fv(glGetUniformLocation(program, "cameraUp"), 1, glm::value_ptr(camUp));

	// Render the object.
	tester->draw();
	

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */

	 // Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_1:
			// Set currentObj to cube
			currentObj = cube;
			break;
		case GLFW_KEY_A:
			//cP->xTransform(-1.0f);
			view = glm::rotate(glm::mat4(1), glm::radians(1.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)) * view;

			break;
		case GLFW_KEY_D:
			//cP->xTransform(1.0f);

			view = glm::rotate(glm::mat4(1), glm::radians(-1.0f),
				glm::vec3(0.0f, 1.0f, 0.0f)) * view;

			break;
		case GLFW_KEY_W:
			//cP->yTransform(-1.0f);
			//while (action == GLFW_PRESS) {
				view = glm::rotate(glm::mat4(1), glm::radians(1.0f),
					glm::vec3(1.0f, 0.0f, 0.0f)) * view;
			//}
			
			break;
		case GLFW_KEY_S:
			//cP->yTransform(1.0f);
				view = glm::rotate(glm::mat4(1), glm::radians(-1.0f),
					glm::vec3(1.0f, 0.0f, 0.0f)) * view;
			break;
		default:
			break;
		}
	}
}
