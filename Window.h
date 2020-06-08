#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "terraingeneration_common.h"

#include "Gui.h";

class Window
{
public:

	Gui* gui;

	Window();
	Window(char* title, int width, int height, int planeLength, int planeWidth);

	bool shouldClose();
	bool intialize();
	float getViewportRatio();
	glm::mat4 getModelMatrix();
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();


	void newFrame();
	void drawFrame();
	
	~Window();

private:
	int width, height;
	int glBufferWidth, glBufferHeight;
	char title[WINDOW_TITLE_LENGTH];
	GLFWwindow* window;

	bool showGui;

	static void keyboardCallback(GLFWwindow* window, int key, int code, int action, int mode);
	static void mouseCallback(GLFWwindow* window, double xPos, double yPos);
	static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
	static void sizeCallback(GLFWwindow*, int width, int height);

	GLfloat lastMouseX, lastMouseY;
	GLfloat mouseXChange, mouseyChange;

	float modelScale;

	int groundplaneLength, groundplaneWidth;

	glm::mat4 projection, view, model;
	glm::vec3 translate_vec;
	GLfloat rotate_angle;
	glm::vec3 rotate_axis;
	glm::vec3 scale_vec;
};

