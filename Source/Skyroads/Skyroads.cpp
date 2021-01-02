
#include "Skyroads.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

Skyroads::Skyroads(){

}

Skyroads::~Skyroads() {

}

void Skyroads::Init() {
	auto camera = GetSceneCamera();
	camera->SetPositionAndRotation(glm::vec3(0, 4, 10), glm::vec3(-10 * TO_RADIANS, 0, 0));
	camera->Update();

	

	{
		/*Shader* shader = new Shader("SurfaceGeneration");
		shader->AddShader("Source/Laboratoare/Skyroads/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Skyroads/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;*/
	}

}

void Skyroads::FrameStart() {

	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0.063f, 0.041f, 0.079f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);

}

void Skyroads::Update(float deltaTimeSeconds) {
	
	

}

void Skyroads::FrameEnd() {
	DrawCoordinatSystem();
}

void Skyroads::OnInputUpdate(float deltaTime, int mods) {

}

void Skyroads::OnKeyPress(int key, int mods) {

}
void Skyroads::OnKeyRelease(int key, int mods) {

}
void Skyroads::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {

}
void Skyroads::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {

}
void Skyroads::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {

}
void Skyroads::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {

}
void Skyroads::OnWindowResize(int width, int height) {

}



