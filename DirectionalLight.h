#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

class DirectionalLight
{
public:
	DirectionalLight(GLuint sId);

	void setColor(GLfloat red, GLfloat green, GLfloat blue);
	void setColor(glm::vec3 color);
	void setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir);
	void setDirection(glm::vec3 direction);
	void setIntensity(GLfloat aIntensity, GLfloat dIntensity);

	void useLight();

	~DirectionalLight();
private:
	GLuint shaderId;
	glm::vec3 direction;
	glm::vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

