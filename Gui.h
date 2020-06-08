#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "terraingeneration_common.h"
#include "glm/glm.hpp"

class Gui
{
public:
	Gui(GLFWwindow* window);

	void showTerrainControls();
	void render();

	float getHeightScalarValue();
	float getNoiseScalarValue();
	float getXOffset();
	float getZOffset();
	float getSteepnessValue();
	int getResolution();
	float getAmbientLightLevel();
	float* getAmbientLightColor();

	float getDirectionalLightAmbientIntensity();
	float getDirectionalLightDiffuseIntensity();
	glm::vec3 getDirectionalLightColor();
	glm::vec3 getDirectionalLightDirection();

	float getRotateSpeed();

	int getRenderType();
	int getViewType();

	void enableInputCapture();
	void disableInputCapture();

	~Gui();

private:
	float height_scalar;
	float noise_scalar;
	float x_offset, z_offset;
	float steepness_exp;
	int resolution;

	float directionalLight_ambientIntensity;
	float directionalLight_diffuseIntensity;
	float directionalLight_color[3];
	float directionalLight_direction[3];

	int renderTypeComboBoxSelection;
	const char* renderTypeComboBoxItems[3] = { (char*)"triangles", (char*)"points", (char*)"lines" };

	int viewTypeComboBoxSelection;
	const char* viewTypeComboBoxItems[2] = { (char*)"side", (char*)"overhead" };


	bool groundplane_rotate;
	float anim_rotate_speed;
};

