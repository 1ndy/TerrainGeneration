#include "Window.h"
#include <string.h>
#include <stdio.h>

Window::Window() {
	width = 800;
	height = 800;
	glBufferWidth = 0;
	glBufferHeight = 0;
	strncpy(title, "Window", WINDOW_TITLE_LENGTH);
	window = nullptr;
	gui = nullptr;
	showGui = true;
	model = glm::mat4(1.0f);
	lastMouseX = 0.0f;
	lastMouseY = 0.0f;

	translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	rotate_angle = 0.0f;
	scale_vec = glm::vec3(1.0f, 1.0f, 1.0f);

	view = glm::lookAt(glm::vec3(10, 10 / 2, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	groundplaneLength = 10;
	groundplaneWidth = 10;

}

Window::Window(char* t, int w, int h, int planeLength, int planeWidth) {
	width = w;
	height = h;
	glBufferWidth = 0;
	glBufferHeight = 0;
	strncpy(title, t, WINDOW_TITLE_LENGTH);
	window = nullptr;
	gui = nullptr;
	showGui = true;
	model = glm::mat4(1.0f);
	lastMouseX = 0.0f;
	lastMouseY = 0.0f;

	modelScale = 1.0f;

	translate_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	rotate_axis = glm::vec3(0.0f, 1.0f, 0.0f);
	rotate_angle = 0.0f;
	scale_vec = glm::vec3(1.0f, 1.0f, 1.0f);

	view = glm::lookAt(glm::vec3(planeLength, planeLength / 2, planeWidth), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	groundplaneLength = planeLength;
	groundplaneWidth = planeWidth;
}

bool Window::intialize() {
	if (!glfwInit()) {
		glfwTerminate();
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	int monitorCount;
	GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
	window = glfwCreateWindow(width, height, (const char*)title, NULL, NULL);

	if (!window) {
		printf("glfw could not create window\n");
		return false;
	}

	glfwGetFramebufferSize(window, &glBufferWidth, &glBufferHeight);

	glfwSwapInterval(1); //fix frame rate to monitor refresh rate

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK) {
		glfwDestroyWindow(window);
		glfwTerminate();
		printf("glfw could not initialize glew\n");
		return false;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, glBufferWidth, glBufferHeight);
	glfwSetWindowUserPointer(window, this);

	projection = glm::perspective(45.0f, this->getViewportRatio(), 0.1f, 10000.0f);

	glfwSetKeyCallback(window, keyboardCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetFramebufferSizeCallback(window, sizeCallback);

	gui = new Gui(window);

	return true;
}

void Window::newFrame()
{
	glfwPollEvents();

	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::drawFrame()
{
	if (showGui) {
		gui->enableInputCapture();
		gui->showTerrainControls();
		gui->render();
	}
	else {
		gui->disableInputCapture();
	}
	glfwSwapBuffers(window);

}

bool Window::shouldClose() {
	return glfwWindowShouldClose(window);
}


float Window::getViewportRatio() {
	return float(glBufferWidth) / float(glBufferHeight);
}

void Window::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
		theWindow->showGui = !theWindow->showGui;
	}
}

void Window::mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	Window* theWindow = static_cast<Window*> (glfwGetWindowUserPointer(window));
	if (!ImGui::GetIO().WantCaptureMouse && glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		theWindow->mouseXChange = xPos - theWindow->lastMouseX;
		theWindow->mouseyChange = theWindow->lastMouseY - yPos;

		theWindow->lastMouseX = xPos;
		theWindow->lastMouseY = yPos;

		theWindow->rotate_angle = theWindow->mouseXChange * toRadians;
	}
}

void Window::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	if (!ImGui::GetIO().WantCaptureMouse) {
		Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		float scale = yOffset / 10.0f;

		theWindow->scale_vec.x += scale;
		theWindow->scale_vec.y += scale;
		theWindow->scale_vec.z += scale;
		theWindow->modelScale += scale;

		if (theWindow->modelScale < 0.5f) {
			theWindow->scale_vec.x = 0.5f;
			theWindow->scale_vec.y = 0.5f;
			theWindow->scale_vec.z = 0.5f;
			theWindow->modelScale = 0.5f;
		}

		if (theWindow->modelScale > 10.0f) {
			theWindow->scale_vec.x = 10.0f;
			theWindow->scale_vec.y = 10.0f;
			theWindow->scale_vec.z = 10.0f;
			theWindow->modelScale = 10.0f;
		}
	}
}

void Window::sizeCallback(GLFWwindow* window, int width, int height) {
	Window* theWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
	theWindow->glBufferWidth = width; 
	theWindow->glBufferHeight = height;
	glViewport(0, 0, width, height);
}

glm::mat4 Window::getModelMatrix() {
	model = glm::translate(model, translate_vec);
	model = glm::rotate(model, rotate_angle, rotate_axis);
	rotate_angle = gui->getRotateSpeed();
	model = glm::scale(model, scale_vec);
	scale_vec = glm::vec3(1.0f, 1.0f, 1.0f);
	return model;
}

glm::mat4 Window::getViewMatrix()
{
	if (gui->getViewType() == 0) {
		view = glm::lookAt(glm::vec3(groundplaneLength, groundplaneLength / 2, groundplaneWidth), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		glm::perspective(45.0f, this->getViewportRatio(), 0.1f, 10000.0f);
	}
	else {
		view = glm::lookAt(glm::vec3(0, groundplaneLength, 0), glm::vec3(0, -1.0f, 0), glm::vec3(1.0f, 0, 0));
		projection = glm::ortho(0, groundplaneWidth, 0, groundplaneLength);
	}
	return view;
}

glm::mat4 Window::getProjectionMatrix()
{
	return projection;
}

Window::~Window() {
	glfwDestroyWindow(window);
	glfwTerminate();
}