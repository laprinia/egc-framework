
#include "Skyroads.h"

#include <vector>
#include <iostream>
#include <cstdlib>
#include <Core/Engine.h>
#include <Engine/Component/Transform/Transform.h>
#include "Rectangle2D.h"

Skyroads::Skyroads() {

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


		float length = 1.90f;
		float width = 0.05f;
		glm::vec3 corner = glm::vec3(-.96, -.96, 0);
		glm::vec3 corner2 = glm::vec3(-.96, -.96, -1);

		Mesh* square1 = Rectangle2D::CreateSquare("square1", corner, width, length, glm::vec3(0, 0, 0), true);
		AddMeshToList(square1);
		Mesh* square2 = Rectangle2D::CreateSquare("square2", corner2, width, length - 0.01f, glm::vec3(0, 0, 0), true);
		AddMeshToList(square2);

	}
	{
		platformColors.push_back(glm::vec3(1 - intensity, 0.51f - intensity, 0.31f - intensity));
		platformColors.push_back(glm::vec3(0.251f - intensity, 0.675f - intensity, 1 - intensity));
		platformColors.push_back(glm::vec3(1 - intensity, 0.4f - intensity, 0.4f - intensity));
		platformColors.push_back(glm::vec3(0.251 - intensity, 1 - intensity, 0.686f - intensity));
		platformColors.push_back(glm::vec3(1 - intensity, 0.765f - intensity, 0.251f - intensity));
		platformColors.push_back(glm::vec3(0.863f - intensity, 1 - intensity, 0.251f - intensity));

	}
	{
		GeneratePlatformData();
		while (!CheckEligibleOnRows()) {
			GeneratePlatformData();
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
			randomIndices[j][i] = (rand() % 6);
		}

	}

}

bool Skyroads::CheckEligibleOnRows() {

	int count = 0;
	int checkCount = 0;
	float frontPlatformStart;
	float spaceBetween;
	if (randomIndices[currentRow][currentLane] == red || randomIndices[currentRow][currentLane] == orange) return false;
	for (int j = 0; j < rowNumber - 1; j++) {
		for (int i = 0; i < laneNumber; i++) {
			if (randomLengths[j][i] == maxPlatformLength && randomIndices[j][i] != red && randomIndices[j+1][i] != red) {
				checkCount++;
				break;
			}
			else {
				frontPlatformStart = platformCenters[j + 1][i].z + randomLengths[j + 1][i] / 2;
				spaceBetween = abs((platformCenters[j][i].z - randomLengths[j][i] / 2) - frontPlatformStart);
				if ((spaceBetween +3 <= jumpableRowSpace) && randomIndices[j][i] != red) {
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

void Skyroads::RenderAMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, bool isUI, bool isPlayer, bool isRandomized) {
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
	glUniform3f(location, color.r, color.g, color.b);

	glUniform1i(glGetUniformLocation(shader->program, "isUI"), isUI);
	glUniform1i(glGetUniformLocation(shader->program, "isRandomized"), isRandomized);
	glUniform1i(glGetUniformLocation(shader->program, "isPlayer"), isPlayer);

	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);

}

void Skyroads::HandleCurrentColorPowerup(int currentColorIndex) {
	switch (currentColorIndex) {
	case red:
		isEndGame = true;
		powerupTimestamp = glfwGetTime();
		break;
	case yellow:
		fuel -= fuel < 0? 0 : 0.20 * fuel;
		powerupTimestamp = glfwGetTime();
		break;
	case orange:
		hasOrangePowerup = true;
		powerupTimestamp = glfwGetTime();
		speed = 10;
		break;
	case green:
		fuel += fuel > 1 ? 0 : 0.20 * fuel;
		powerupTimestamp = glfwGetTime();
		break;

	}
}
bool Skyroads::CheckCollisionBetweenTwo(glm::vec3 onePosition, glm::vec3 oneSize, glm::vec3 twoPosition, glm::vec3 twoSize)
{
	
		bool collisionX = onePosition.x + oneSize.x >= twoPosition.x &&
			twoPosition.x + twoSize.x >= onePosition.x;
		
		bool collisionY = onePosition.y + oneSize.y >= twoPosition.y &&
			twoPosition.y + twoSize.y >= onePosition.y;
		
		bool collisionZ = onePosition.z + oneSize.z >= twoPosition.z &&
			twoPosition.z + twoSize.y >= onePosition.z;

		return collisionZ && collisionY && collisionX;
	
}
std::string Skyroads::getPlatformColorName(int indexInEnum)
{
	switch (indexInEnum) {
	case red:
		return "red";
		break;
	case yellow:
		return "yellow";
		break;
	case orange:
		return "orange";
		break;
	case green:
		return "green";
		break;
	case blue:
		return "blue";
		break;
	case lime:
		return "lime";
		break;
	}
}
void Skyroads::GeneratePlatforms(int zOffset) {
	int k = 0;
	for (int j = 0; j < rowNumber; j++) {
		int startIndex = -12;
		for (int i = 0; i < laneNumber; i++) {

			glm::mat4 modelMatrix = glm::mat4(1);

			glm::vec3 minusGravityCenter = glm::vec3(-(startIndex + 3), 0.5f, -(zOffset + randomLengths[j][i] / 2));
			platformCenters[j][i] = minusGravityCenter;
			modelMatrix = glm::translate(modelMatrix, minusGravityCenter);
			modelMatrix = glm::scale(modelMatrix, glm::vec3(6.0f, 1.0f, randomLengths[j][i]));
			bool isVisited = (currentLane == i) && (currentRow == j);

			if (!(j < currentRow)) {
				RenderAMesh(meshes["box"], shaders["Skyroads"], modelMatrix, isVisited ? glm::vec3(0.639f - intensity, 0.251f - intensity, 1 - intensity) : platformColors[randomIndices[j][i]], false, false, false);
			}
			if (CheckCollisionBetweenTwo(glm::vec3(sphereCoordonate.x,sphereCoordonate.y-2,sphereCoordonate.z+1), glm::vec3(2.0f), minusGravityCenter, glm::vec3(6.0f, 1.0f, randomLengths[j][i]))) {
				std::cout << "Sphere player collided w/ platform colored "<< getPlatformColorName(randomIndices[j][i]) << std::endl;
			}

			if (isVisited && canGetPowerup) {
				HandleCurrentColorPowerup(randomIndices[j][i]);
			}
			startIndex += randomWidths[j][i];
			k++;

		}
		zOffset += maxPlatformLength + 1;
	}

}

void Skyroads::Update(float deltaTimeSeconds) {


	GeneratePlatforms(0);
	canGetPowerup = false;

	glm::mat4 modelMatrix = glm::mat4(1);
	float platformX = (float)platformCenters[currentRow][currentLane].x;
	float platformZ = (float)platformCenters[currentRow][currentLane].z;
	bool isPowerupExpired = glfwGetTime() >= powerupTimestamp + powerupTimeToWait;

	if (hasOrangePowerup && isPowerupExpired) {
		speed = 1;
		hasOrangePowerup = false;
	}

	bool hasFallen = sphereCoordonate.z < platformZ - randomLengths[currentRow][currentLane] / 2 || isEndGame;

	sphereCoordonate = hasFallen ? glm::vec3(sphereCoordonate.x, sphereCoordonate.y - 2.7f * deltaTimeSeconds, sphereCoordonate.z) : glm::vec3(platformX, 2.0f, -1.0f - translateZ);
	modelMatrix = glm::translate(modelMatrix, sphereCoordonate);
	modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f));

	if (sphereCoordonate.y >= -10) {
		Skyroads::RenderAMesh(meshes["sphere"], shaders["Skyroads"], modelMatrix, playerColor, false, true, isPowerupExpired ? false : true);
	}
	

	if (!hasFallen && !isEndGame) {


		float offset = deltaTimeSeconds * speed;
		translateZ += offset;
		fuel -= offset * 0.009f;
		auto camera = GetSceneCamera();
		camera->SetPositionAndRotation(glm::vec3(sphereCoordonate.x, sphereCoordonate.y + (isFirstPerson ? 0 : 4), sphereCoordonate.z + (isFirstPerson ? -2 : 10)), glm::vec3((isFirstPerson ? 5 : -10) * TO_RADIANS, 0, 0));
		camera->Update();
	}
	else {

		isEndGame = true;
	}

	modelMatrix = glm::mat4(1);
	Skyroads::RenderAMesh(meshes["square1"], shaders["Skyroads"], modelMatrix, playerColor, true, false, false);

	modelMatrix = glm::scale(modelMatrix, glm::vec3(fuel, 1, 1));
	if (fuel <= 0) {
		isEndGame = true;
	}
	else {
		Skyroads::RenderAMesh(meshes["square2"], shaders["Skyroads"], modelMatrix, fuelColor, true, false, false);
	}
}

void Skyroads::FrameEnd() {
	//DrawCoordinatSystem();
}

void Skyroads::OnInputUpdate(float deltaTime, int mods) {

	float offset = deltaTime * speed;
	if (window->KeyHold(GLFW_KEY_W) && !hasOrangePowerup) {

		speed += 0.4f;

	}
	else if (window->KeyHold(GLFW_KEY_S) && !hasOrangePowerup) {

		if (speed > 1) {
			speed -= 0.4f;
		}

	}
}

void Skyroads::OnKeyPress(int key, int mods) {
	if (!isEndGame) {
		if (window->KeyHold(GLFW_KEY_A)) {
			if ((currentLane < laneNumber) && (randomWidths[currentRow][currentLane] - 6 <= jumpableLaneSpace)) {
				currentLane += 1;
				canGetPowerup = true;
			}

		}
		else if (window->KeyHold(GLFW_KEY_D)) {
			if ((currentLane > 0) && (randomWidths[currentRow][currentLane - 1] - 6 <= jumpableLaneSpace)) {
				currentLane -= 1;
				canGetPowerup = true;
			}

		}
		else if (window->KeyHold(GLFW_KEY_SPACE)) {
			float frontPlatformStart = platformCenters[currentRow + 1][currentLane].z + randomLengths[currentRow + 1][currentLane] / 2;
			float spaceBetween = abs(sphereCoordonate.z - frontPlatformStart);

			if (spaceBetween <= jumpableRowSpace) {
				currentRow += 1;
				translateZ += spaceBetween;
				canGetPowerup = true;

			}
		}
		else if (window->KeyHold(GLFW_KEY_C)) {
			isFirstPerson = !isFirstPerson;

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



