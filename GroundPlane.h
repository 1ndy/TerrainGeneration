#pragma once

#include <noise/noise.h>
#include <GL/glew.h>

class GroundPlane
{
public:
	GroundPlane();
	GroundPlane(GLfloat x, GLfloat z);

	void createMesh();

	void render(int type);

	~GroundPlane();
private:
	GLfloat width, length;
	GLint numVertices;
	GLint numIndices;
	GLfloat* mesh;
	GLuint* indices;

	GLuint vao, vbo, ibo;

	void computeNumVertices();
};

