#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(GLuint sId) 
{
	shaderId = sId;
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	direction = glm::vec3(0.0f, 1.0f, 0.0f);
	ambientIntensity = 0.5f;
	diffuseIntensity = 0.5f;
}

void DirectionalLight::setColor(GLfloat red, GLfloat green, GLfloat blue)
{
	color = glm::vec3(red, green, blue);
}

void DirectionalLight::setColor(glm::vec3 col)
{
	color = col;
}

void DirectionalLight::setDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::setDirection(glm::vec3 dir)
{
	direction = dir;
}

void DirectionalLight::setIntensity(GLfloat aIntensity, GLfloat dIntensity)
{
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}

void DirectionalLight::useLight() {
	GLuint ai_loc = glGetUniformLocation(shaderId, "dirlight.ambientIntensity");
	glUniform1f(ai_loc, ambientIntensity);

	GLuint di_loc = glGetUniformLocation(shaderId, "dirlight.diffuseIntensity");
	glUniform1f(di_loc, diffuseIntensity);

	GLuint lc_loc = glGetUniformLocation(shaderId, "dirlight.color");
	glUniform3f(lc_loc, color.x, color.y, color.z);

	GLuint ld_loc = glGetUniformLocation(shaderId, "dirlight.direction");
	glUniform3f(ld_loc, direction.x, direction.y, direction.z);
}

DirectionalLight::~DirectionalLight() {};