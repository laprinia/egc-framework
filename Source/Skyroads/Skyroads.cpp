
#include "Skyroads.h"

#include <vector>
#include <iostream>
#include <cstdlib>
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
		Shader* shader = new Shader("Skyroads");
		shader->AddShader("Source/Skyroads/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Skyroads/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
	}

	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}
	{
		
		for (int i = 0; i < laneNumber * rowNumber;i++) {
			randomWidths.push_back( GenerateRandomWidth());
			randomLengths.push_back( GenerateRandomLength());
		}
	}

}

void Skyroads::FrameStart() {


	glClearColor(0.37f, 0.058f, 0.04f, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
	glViewport(0, 0, resolution.x, resolution.y);

}
int Skyroads::GenerateRandomLength() {
	return rand() % (maxPlatformLength - minPlatformLength + 1) + minPlatformLength;
}
int Skyroads::GenerateRandomWidth() {
	return rand() % (maxPlatformWidth - minPlatformWidth + 1) + minPlatformWidth;
}
void Skyroads::GenerateRowOfPlatforms(int zOffset ) {
	int k = 0;
	
	
	for (int j = 0; j < rowNumber; j++) {
		int startIndex = -12;
		for (int i = 0; i < laneNumber; i++) {

			glm::mat4 modelMatrix = glm::mat4(1);


			modelMatrix = glm::translate(modelMatrix, glm::vec3(-(startIndex + 3), 0.5f, -(zOffset + randomLengths[k] / 2)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 1.0f, randomLengths[k]));

			RenderMesh(meshes["box"], shaders["VertexNormal"], modelMatrix);


			startIndex += randomWidths[k];
			k++;
		}
		zOffset += maxPlatformLength;
	}
	
}
void Skyroads::Update(float deltaTimeSeconds) {
	GenerateRowOfPlatforms(0);
	
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



