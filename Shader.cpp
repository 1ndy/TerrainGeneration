#include "Shader.h"
#include <stdio.h>
#include <fstream>

Shader::Shader() {
	shaderId = 0;
}

bool Shader::loadShaderCode(char* vert_fname, char* geo_fname, char* frag_fname) {
	std::string vc = readShaderCode(vert_fname);
	std::string gc = readShaderCode(geo_fname);
	std::string fc = readShaderCode(frag_fname);

	if (vc != "" && gc != "" && fc != "") {
		return compileShaderCode(vc, gc, fc);
	}
	else {
		return false;
	}
}

void Shader::useShader() {
	glUseProgram(shaderId);
}

std::string Shader::readShaderCode(char* fname) {
	printf("starting readShaderCode for %s\n", fname);
	std::string shaderCode = std::string();
	std::ifstream shaderFile;
	shaderFile.open(fname);
	if (!shaderFile.is_open()) {
		printf("failed to open shaderfile '%s'\n", fname);
		return std::string();
	}

	std::string line = std::string();
	while (!shaderFile.eof()) {
		std::getline(shaderFile, line);
		shaderCode.append(line);
		shaderCode.append("\n");
		//printf("%s\n", line.c_str());
	}
	printf("finished reading shader %s\n", fname);

	shaderFile.close();

	return shaderCode;
}

bool Shader::compileShaderCode(std::string vcode, std::string gcode, std::string fcode) {
	shaderId = glCreateProgram();

	if (!shaderId) {
		printf("Could not allocate shader on GPU\n");
		return false;
	}
	GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
	GLuint gshader = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);

	//dumb for 1 shader
	const GLchar* sCode[1];
	GLint sSize[1];
	sCode[0] = vcode.c_str();
	sSize[0] = (GLint)vcode.length();
	glShaderSource(vshader, 1, sCode, sSize);

	sCode[0] = gcode.c_str();
	sSize[0] = (GLint)gcode.length();
	glShaderSource(gshader, 1, sCode, sSize);

	sCode[0] = fcode.c_str();
	sSize[0] = (GLint)fcode.length();
	glShaderSource(fshader, 1, sCode, sSize);

	glCompileShader(vshader);
	glCompileShader(gshader);
	glCompileShader(fshader);

	GLint vresult, gresult, fresult;
	GLchar message[1024] = { 0 };

	glGetProgramiv(vshader, GL_COMPILE_STATUS, &vresult);
	glGetProgramiv(gshader, GL_COMPILE_STATUS, &gresult);
	glGetProgramiv(vshader, GL_COMPILE_STATUS, &fresult);

	if (!vresult) {
		glGetShaderInfoLog(vshader, 1024, NULL, message);
		printf("Shader %d Vertex shader error: %s\n", shaderId, message);
		return false;
	}
	
	if (!gresult) {
		glGetShaderInfoLog(gshader, 1024, NULL, message);
		printf("Shader %d Geometry shader error: %s\n", shaderId, message);
		return false;
	}

	if (!fresult) {
		glGetShaderInfoLog(fshader, 1024, NULL, message);
		printf("Shader %d Fragment shader error: %s\n", shaderId, message);
		return false;
	}

	glAttachShader(shaderId, vshader);
	glAttachShader(shaderId, gshader);
	glAttachShader(shaderId, fshader);

	glLinkProgram(shaderId);

	GLint result;
	glGetProgramiv(shaderId, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderId, 1024, NULL, message);
		glDeleteProgram(shaderId);
		printf("Failed to link shader %d: %s\n", shaderId, message);
		return false;
	}

	glValidateProgram(shaderId);
	glGetProgramiv(shaderId, GL_VALIDATE_STATUS, &result);
	if (!result) {
		glGetShaderInfoLog(shaderId, 1024, NULL, message);
		glDeleteProgram(shaderId);
		printf("Failed to validate shader %d: %s\n", shaderId, message);
	}

	return true;

}

void Shader::setProjectionUniform(glm::mat4 projectionMatrix) {
	projectionUniform = glGetUniformLocation(shaderId, "projection");
	glUniformMatrix4fv(projectionUniform, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Shader::setViewUniform(glm::mat4 viewMatrix) {
	viewUniform = glGetUniformLocation(shaderId, "view");
	glUniformMatrix4fv(viewUniform, 1, GL_FALSE, glm::value_ptr(viewMatrix));
}

void Shader::setModelUniform(glm::mat4 modelMatrix)
{
	GLuint mm_loc = glGetUniformLocation(shaderId, "model");
	glUniformMatrix4fv(mm_loc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Shader::setHeightScalarUniform(float heightScalar)
{
	GLuint hs_loc = glGetUniformLocation(shaderId, "height_scalar");
	glUniform1f(hs_loc, heightScalar);
}

void Shader::setNoiseScalarUniform(float noiseScalar)
{
	GLuint ns_loc = glGetUniformLocation(shaderId, "noise_scalar");
	glUniform1f(ns_loc, noiseScalar);
}

void Shader::setNoiseXOffsetUniform(float xOffset)
{
	GLuint xo_loc = glGetUniformLocation(shaderId, "x_offset");
	glUniform1f(xo_loc, xOffset);
}

void Shader::setNoiseZOffsetUniform(float zOffset)
{
	GLuint zo_loc = glGetUniformLocation(shaderId, "z_offset");
	glUniform1f(zo_loc, zOffset);
}

void Shader::setSteepnessExponentUniform(float steepness)
{
	GLuint se_loc = glGetUniformLocation(shaderId, "steepness_exp");
	glUniform1f(se_loc, steepness);
}

void Shader::setResolution(int resolution)
{
	GLuint res_loc = glGetUniformLocation(shaderId, "resolution");
	glUniform1i(res_loc, resolution);
}

GLuint Shader::getShaderId()
{
	return shaderId;
}

Shader::~Shader() {
	if(shaderId != 0) glDeleteProgram(shaderId);
}