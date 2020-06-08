#include "GroundPlane.h"
#include <stdio.h>

GroundPlane::GroundPlane() {
	width = 10.0f;
	length = 10.0f;
	mesh = nullptr;
	indices = nullptr;
	vao = 0, vbo = 0, ibo = 0;
	computeNumVertices();
}

GroundPlane::GroundPlane(GLfloat x, GLfloat z) {
	width = x;
	length = z;
	mesh = nullptr;
	indices = nullptr;
	vao = 0, vbo = 0, ibo = 0;
	computeNumVertices();
}

void GroundPlane::computeNumVertices() {
	numVertices = length * width;
	numIndices = (length - 1) * (width - 1) * 6;
}

void GroundPlane::createMesh() {
	printf("creating mesh\n");
	//define the mesh
	int numPoints = numVertices * 3;
	mesh = new GLfloat[numPoints];
	indices = new GLuint[numIndices];
	
	int k = 0;
	for (int i = length / 2; i > length / -2; i--) {
		for (int j = width / -2; j < width / 2; j++) {
			mesh[k] = j;
			mesh[k + 1] = 0.0f;
			mesh[k + 2] = i;
			//printf("(%.2f %.2f %.2f)\t", mesh[k], mesh[k + 1], mesh[k + 2]);
			k += 3;
		}
		//printf("\n");
	}
	printf("done filling mesh\n");


	//indexes
	int p = 0;
	int startcol = 0;
	for (int i = 0; i < length-1; i++) {
		for (int j = 0; j < width-1; j++) {
			indices[p] = startcol;
			indices[p + 1] = startcol + width;
			indices[p + 2] = startcol + 1;
			//printf("(%d %d %d)  ", indices[p], indices[p + 1], indices[p + 2]);
			
			indices[p + 3] = startcol + 1;
			indices[p + 4] = startcol + width;
			indices[p + 5] = startcol + width + 1;
			//printf("(%d %d %d)\t", indices[p+3], indices[p + 4], indices[p + 5]);
			p += 6;
			startcol++;
		}
		//printf("\n");
		startcol++;
	}
	printf("done filling indexes\n");
	

	//copy it to the gpu
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh[0]) * numVertices * 3, mesh, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(mesh[0]) * 3, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}

void GroundPlane::render(int type) {
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	if (type == 0) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}
	else if (type == 1) {
		glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_INT, 0);
	} else {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
	}
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GroundPlane::~GroundPlane() {
	if (ibo != 0) {
		glDeleteBuffers(1, &ibo);
	}

	if (vao != 0) {
		glDeleteBuffers(1, &vao);
	}

	if (vbo != 0) {
		glDeleteBuffers(1, &vbo);
	}
	
	mesh = nullptr;
	indices = nullptr;
}