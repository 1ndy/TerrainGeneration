#pragma once
#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shader
{
public:
	Shader();
	bool loadShaderCode(char* vertex_filename, char* geo_fname, char* frag_filename);

	void setProjectionUniform(glm::mat4 projectionMatrix);
	void setViewUniform(glm::mat4 viewMatrix);
	void setModelUniform(glm::mat4 modelMatrix);
	void setHeightScalarUniform(float heightScalar);
	void setNoiseScalarUniform(float noiseScalar);
	void setNoiseXOffsetUniform(float xOffset);
	void setNoiseZOffsetUniform(float zOffset);
	void setSteepnessExponentUniform(float steepness);
	void setResolution(int resolution);

	GLuint getShaderId();

	void useShader();
	~Shader();
private:
	GLuint shaderId;
	GLuint viewUniform, projectionUniform;
	std::string readShaderCode(char* filename);
	bool compileShaderCode(std::string vertex_shaderCode, std::string geo_shaderCode, std::string frag_shaderCode);
};

