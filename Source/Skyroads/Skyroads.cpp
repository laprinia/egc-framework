
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
	camera->SetPositionAndRotation(glm::vec3(0, 4, 6), glm::vec3(-10 * TO_RADIANS, 0, 0));
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

		Mesh* mesh2 = new Mesh("sphere");
		mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh2->GetMeshID()] = mesh2;
	}
	{
		platformColors.push_back(glm::vec3(1 - intensity, 0.51f-intensity, 0.31f - intensity));
		platformColors.push_back(glm::vec3(0.251f - intensity, 0.675f - intensity, 1 - intensity));
		platformColors.push_back(glm::vec3(0.639f - intensity, 0.251f - intensity, 1 - intensity));
		platformColors.push_back(glm::vec3(0.251 - intensity, 1 - intensity, 0.686f - intensity));
		platformColors.push_back(glm::vec3(1 - intensity, 0.765f - intensity, 0.251f - intensity));
		platformColors.push_back(glm::vec3(0.863f - intensity, 1 - intensity, 0.251f - intensity));
			
	}
	{
		
		for (int i = 0; i < laneNumber * rowNumber;i++) {
			randomWidths.push_back( GenerateRandomWidth());
			randomLengths.push_back( GenerateRandomLength());
			randomIndices.push_back(rand() % 6);
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

void Skyroads::RenderAMesh(Mesh* mesh, Shader* shader, const glm::mat4 &modelMatrix, const glm::vec3 &color) {
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	glUseProgram(shader->program);
	int location = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

	location = glGetUniformLocation(shader->program, "View");
	glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

	location = glGetUniformLocation(shader->program, "Projection");
	glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

	location = glGetUniformLocation(shader->program, "color");
	glUniform3f(location, color.r,color.g,color.b);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

}
void Skyroads::GeneratePlatforms(int zOffset ) {
	int k = 0;
	
	
	for (int j = 0; j < rowNumber; j++) {
		int startIndex = -12;
		for (int i = 0; i < laneNumber; i++) {

			glm::mat4 modelMatrix = glm::mat4(1);


			modelMatrix = glm::translate(modelMatrix, glm::vec3(-(startIndex + 3), 0.5f, -(zOffset + randomLengths[k] / 2)));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 1.0f, randomLengths[k]));
			
			RenderAMesh(meshes["box"], shaders["Skyroads"], modelMatrix, platformColors[randomIndices[k]]);


			startIndex += randomWidths[k];
			k++;
			
		}
		zOffset += maxPlatformLength;
	}
	
}
void Skyroads::Update(float deltaTimeSeconds) {
	GeneratePlatforms(0);

	glm::mat4 modelMatrix = glm::mat4(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 2.0f, -1.0f));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	Skyroads::RenderAMesh(meshes["sphere"], shaders["Skyroads"], modelMatrix, playerColor);
	
}

void Skyroads::FrameEnd() {
	//DrawCoordinatSystem();
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



