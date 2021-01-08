
#include "Skyroads.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <Core/Engine.h>
#include <Engine/Component/Transform/Transform.h>

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
		GeneratePlatformData();
		while (!CheckEligibleOnRows() && !CheckEligibleOnLanes()) {
			GeneratePlatformData();
			std::cout << "no solution" << std::endl;
		}
	}
	{
		speed = 1;
		translateZ = 0;
	}

}

void Skyroads::GeneratePlatformData() {

	for (int j = 0; j < rowNumber; j++) {
		for (int i = 0; i < laneNumber; i++) {
			randomWidths[j][i] = GenerateRandomWidth();
			randomLengths[j][i] = GenerateRandomLength();
		}

	}
	for (int i = 0; i < laneNumber * rowNumber; i++) {
		randomIndices.push_back(rand() % 6);
	}
}
bool Skyroads::CheckEligibleOnRows() {
	int count = 0;
	int checkCount = 0;
	float frontPlatformStart;
	float spaceBetween;
	for (int j = 0; j < rowNumber - 1; j++) {
		for (int i = 0; i < laneNumber; i++) {
			if (randomLengths[j][i] == maxPlatformLength) {
				checkCount++;
				break;
			}
			else {
				frontPlatformStart = platformCenters[j + 1][i].z + randomLengths[j + 1][i] / 2;
				spaceBetween = abs((platformCenters[j][i].z - randomLengths[j][i] / 2) - frontPlatformStart);
				if (spaceBetween <= jumpableRowSpace) {
					checkCount++;
					break;
				}
			}
		}
	}
	return checkCount >= rowNumber - 1;
}
bool Skyroads::CheckEligibleOnLanes() {
	int count = 0;
	int checkCount = 0;
	float frontPlatformStart;
	float spaceBetween;
	for (int j = 0; j < rowNumber; j++) {
		for (int i = 0; i < laneNumber; i++) {
			if (randomWidths[j][i] - 6 <= jumpableLaneSpace) {
				checkCount++;
				break;
			}
		}

	}
	return checkCount >= rowNumber;
}

void Skyroads::FrameStart() {


	glClearColor(0.137f, 0.102f, 0.2f, 0);
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

	int location = glGetUniformLocation(shader->program, "light_position");
	glUniform3fv(location, 1, glm::value_ptr(lightPosition));

	glm::vec3 eyePosition = GetSceneCamera()->transform->GetWorldPosition();
	location = glGetUniformLocation(shader->program, "eye_position");
	glUniform3fv(location, 1, glm::value_ptr(eyePosition));

	location = glGetUniformLocation(shader->program, "material_shininess");
	glUniform1i(location, materialShininess);

	location = glGetUniformLocation(shader->program, "material_kd");
	glUniform1f(location, materialKd);

	location = glGetUniformLocation(shader->program, "material_ks");
	glUniform1f(location, materialKs);

	location = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(location, 1, glm::value_ptr(color));

	location = glGetUniformLocation(shader->program, "Model");
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

			glm::vec3 gravityCenter = glm::vec3(-(startIndex + 3), 0.5f, -(zOffset + randomLengths[j][i] / 2));
			platformCenters[j][i] = gravityCenter;
			modelMatrix = glm::translate(modelMatrix, gravityCenter);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 1.0f, randomLengths[j][i]));
			
			RenderAMesh(meshes["box"], shaders["Skyroads"], modelMatrix, platformColors[randomIndices[k]]);

			startIndex += randomWidths[j][i];
			k++;
			
		}
		zOffset += maxPlatformLength+1;
	}
	
}
void Skyroads::Update(float deltaTimeSeconds) {
	GeneratePlatforms(0);

	glm::mat4 modelMatrix = glm::mat4(1);
	float platformX = (float)platformCenters[currentRow][currentLane].x;
	float platformZ = (float)platformCenters[currentRow][currentLane].z;

	float offset = deltaTimeSeconds * speed;
     translateZ += offset;
	sphereCoordonate= glm::vec3(platformX, 2.0f, -1.0f - translateZ);
	modelMatrix = glm::translate(modelMatrix,sphereCoordonate);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));
	
	if (sphereCoordonate.z > platformZ-randomLengths[currentRow][currentLane]/2) {
		Skyroads::RenderAMesh(meshes["sphere"], shaders["Skyroads"], modelMatrix, playerColor);
		auto camera = GetSceneCamera();
		camera->SetPositionAndRotation(glm::vec3(sphereCoordonate.x, sphereCoordonate.y + 4, sphereCoordonate.z + 10), glm::vec3(-10 * TO_RADIANS, 0, 0));
		camera->Update();
	}
	
	
}

void Skyroads::FrameEnd() {
	//DrawCoordinatSystem();
}

void Skyroads::OnInputUpdate(float deltaTime, int mods) {

	float offset = deltaTime * speed;
	if (window->KeyHold(GLFW_KEY_W)) {
	
		speed += 0.4f;
		
	}
	else if (window->KeyHold(GLFW_KEY_S)) {
	
		if (speed > 1) {
			speed -= 0.4f;
		}
		
	}
	
	
}

void Skyroads::OnKeyPress(int key, int mods) {
	if (window->KeyHold(GLFW_KEY_A)) {
		if ((currentLane < laneNumber) &&(randomWidths[currentRow][currentLane] - 6 <= jumpableLaneSpace)) {
			currentLane += 1;
		}
	 
	}
	else if (window->KeyHold(GLFW_KEY_D)) {
		if ((currentLane > 0)&& (randomWidths[currentRow][currentLane-1] - 6 <= jumpableLaneSpace)) {
			currentLane -= 1;
		}
	 
	}
	else if (window->KeyHold(GLFW_KEY_SPACE)) {
		float frontPlatformStart= platformCenters[currentRow+1][currentLane].z+randomLengths[currentRow+1][currentLane]/2;
		float spaceBetween = abs(sphereCoordonate.z - frontPlatformStart);

		if (spaceBetween <= jumpableRowSpace) {
			currentRow += 1;
			translateZ+= spaceBetween;

		}
	}
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



