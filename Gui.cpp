#include "Gui.h"

Gui::Gui(GLFWwindow* win) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(win, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	height_scalar = 0.0f;
	noise_scalar = 0.5f;
	x_offset = 0.0f;
	z_offset = 0.0f;
	steepness_exp = 1;
	resolution = 4;

	directionalLight_ambientIntensity = 0.5f;
	directionalLight_diffuseIntensity = 0.5f;
	directionalLight_color[0] = 1.0f;
	directionalLight_color[1] = 1.0f;
	directionalLight_color[2] = 1.0f;
	directionalLight_direction[0] = -1.0f;
	directionalLight_direction[1] = 0.0f;
	directionalLight_direction[2] = 0.0f;



	renderTypeComboBoxSelection = 0;
	viewTypeComboBoxSelection = 0;

	groundplane_rotate = false;
	anim_rotate_speed = 0.0f;

}

void Gui::showTerrainControls() {
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("Controls");
		ImGui::BeginTabBar("Controller");

		if (ImGui::BeginTabItem("Terrain")) {
			ImGui::SliderFloat("Height Scalar", &height_scalar, 0.0f, 100.0f);
			ImGui::SliderFloat("Noise Scalar", &noise_scalar, 0.0f, 1.0f);
			ImGui::SliderFloat("Steepness", &steepness_exp, 0.0f, 3.0f);
			ImGui::SliderFloat("X Offset", &x_offset, -1000.0f, 1000.0f);
			ImGui::SliderFloat("Z Offset", &z_offset, -1000.0f,1000.0f);
			ImGui::SliderInt("Resolution", &resolution, 0, 20);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Animation")) {
			ImGui::Checkbox("Rotate", &groundplane_rotate);
			if (groundplane_rotate) {
				ImGui::SliderFloat("Rotate speed", &anim_rotate_speed, -2.0f, 2.0f);
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Lighting")) {
			ImGui::SliderFloat("Ambient Light", &directionalLight_ambientIntensity, 0.0f, 1.0f);
			ImGui::SliderFloat("Diffuse Light", &directionalLight_diffuseIntensity, 0.0f, 1.0f);
			ImGui::ColorEdit3("Color", directionalLight_color);
			ImGui::SliderFloat3("Direction", directionalLight_direction, -1.0f, 1.0f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("View")) {
			if (ImGui::BeginCombo("Render type",renderTypeComboBoxItems[renderTypeComboBoxSelection])) {
				for (int i = 0; i < IM_ARRAYSIZE(renderTypeComboBoxItems); i++) {
					if (ImGui::Selectable(renderTypeComboBoxItems[i], renderTypeComboBoxSelection == i)) {
						renderTypeComboBoxSelection = i;
					}
				}
				ImGui::EndCombo();
			}
			
			
			if (ImGui::BeginCombo("View", viewTypeComboBoxItems[viewTypeComboBoxSelection])) {
				for (int i = 0; i < IM_ARRAYSIZE(viewTypeComboBoxItems); i++) {
					if (ImGui::Selectable(viewTypeComboBoxItems[i], viewTypeComboBoxSelection == i)) {
						viewTypeComboBoxSelection = i;
					}

				}
				ImGui::EndCombo();
			}
			
			ImGui::EndTabItem();
		
		}

		if (ImGui::BeginTabItem("Info")) {
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
		ImGui::End();
	}
}

int Gui::getRenderType() {
	return renderTypeComboBoxSelection;
}

int Gui::getViewType()
{
	return viewTypeComboBoxSelection;
}

void Gui::enableInputCapture()
{
	ImGui::GetIO().WantCaptureMouse = true;
}

void Gui::disableInputCapture()
{
	ImGui::GetIO().WantCaptureMouse = false;
}
void Gui::render() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

float Gui::getHeightScalarValue()
{
	return height_scalar;
}

float Gui::getNoiseScalarValue()
{
	return noise_scalar;
}

float Gui::getXOffset()
{
	return x_offset;
}

float Gui::getZOffset()
{
	return z_offset;
}

float Gui::getSteepnessValue()
{
	return steepness_exp;
}

int Gui::getResolution()
{
	return resolution;
}

float Gui::getDirectionalLightAmbientIntensity()
{
	return directionalLight_ambientIntensity;
}

float Gui::getDirectionalLightDiffuseIntensity()
{
	return directionalLight_diffuseIntensity;
}

glm::vec3 Gui::getDirectionalLightColor()
{
	return glm::vec3(directionalLight_color[0], directionalLight_color[1], directionalLight_color[2]);
}

glm::vec3 Gui::getDirectionalLightDirection()
{
	return glm::vec3(directionalLight_direction[0], directionalLight_direction[1], directionalLight_direction[2]);
}

float Gui::getRotateSpeed()
{
	if (groundplane_rotate) {
		return anim_rotate_speed * toRadians;
	}
	else {
		return 0.0f;
	}
}

Gui::~Gui() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}