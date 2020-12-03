#include "TemaLaborator1.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>

using namespace std;

glm::vec3 screenColor;
unsigned int renderableType;
glm::vec3 renderPosition;



// Order of function calling can be seen in "Source/Core/World.cpp::LoopUpdate()"
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/World.cpp

TemaLaborator1::TemaLaborator1()
{
}

TemaLaborator1::~TemaLaborator1()
{
}

void TemaLaborator1::Init()
{
	// Load a mesh from file into GPU memory

	renderPosition = glm::vec3(1, 1, 1);
	renderableType = 1;
	screenColor = glm::vec3(0, 0, 0);
	Mesh* mesh = new Mesh("box");
	Mesh* mesh2 = new Mesh("bamboo");
	Mesh* mesh3 = new Mesh("teapot");
	Mesh* mesh4 = new Mesh("sphere");
	mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
	meshes[mesh->GetMeshID()] = mesh;
	mesh2->LoadMesh(RESOURCE_PATH::MODELS + "Vegetation/Bamboo", "bamboo.obj");
	meshes[mesh2->GetMeshID()] = mesh2;
	mesh3->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "teapot.obj");
	meshes[mesh3->GetMeshID()] = mesh3;
	mesh4->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
	meshes[mesh4->GetMeshID()] = mesh4;
	
}

void TemaLaborator1::FrameStart()
{
}

void TemaLaborator1::Update(float deltaTimeSeconds)
{
	glm::ivec2 resolution = window->props.resolution;

	// sets the clear color for the color buffer
	glClearColor(screenColor.x, screenColor.y, screenColor.z, 1);

	// clears the color buffer (using the previously set color) and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
	// render the object

	RenderMesh(meshes["box"], renderPosition, glm::vec3(0.5f));

	RenderMesh(meshes["bamboo"], glm::vec3(2, 0, 0), glm::vec3(0.08f));
	RenderMesh(meshes["bamboo"], glm::vec3(0, 0, 0), glm::vec3(0.06f));

	// render the object again but with different properties
	switch (renderableType) {
	case 1:
		RenderMesh(meshes["box"], glm::vec3(-1, 0.5f, 0)); break;
	case 2:
		RenderMesh(meshes["teapot"], glm::vec3(-1, 0.5f, 0)); break;
	case 3:
		RenderMesh(meshes["sphere"], glm::vec3(-1, 0.5f, 0)); break;
	}

}

void TemaLaborator1::FrameEnd()
{
	DrawCoordinatSystem();
}

// Read the documentation of the following functions in: "Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

void TemaLaborator1::OnInputUpdate(float deltaTime, int mods)
{
	float offset = deltaTime * 2;

	if (window->KeyHold(GLFW_KEY_W)) {
		renderPosition -= glm::vec3(0, 0, offset);
	}else if (window->KeyHold(GLFW_KEY_A)) {
		renderPosition -= glm::vec3(offset, 0, 0);
	}else if (window->KeyHold(GLFW_KEY_S)) {
		renderPosition += glm::vec3(0, 0, offset);
	}else if (window->KeyHold(GLFW_KEY_D)) {
		renderPosition += glm::vec3(offset, 0, 0);
	}else if (window->KeyHold(GLFW_KEY_E)) {
		renderPosition -= glm::vec3(0, offset,0);
	}else if (window->KeyHold(GLFW_KEY_Q)) {
		renderPosition += glm::vec3(0, offset, 0);
	}
	
};

void TemaLaborator1::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_R) {

		if (screenColor != glm::vec3(1, 0, 0)) {
			screenColor = glm::vec3(1, 0, 0);
		}
		else {
			screenColor = glm::vec3(0, 0, 0);
		}
	}
	else if (key == GLFW_KEY_X) {
		switch (renderableType) {
		case 1:
			renderableType = 2; break;
		case 2:
			renderableType = 3; break;
		case 3:
			renderableType = 1; break;

		}

	}
};

void TemaLaborator1::OnKeyRelease(int key, int mods)
{
	// add key release event
};

void TemaLaborator1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
};

void TemaLaborator1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
};

void TemaLaborator1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void TemaLaborator1::OnMouseScroll(int mouseX, int mouseY, int renderPositionX, int renderPositionY)
{
	// treat mouse scroll event
}

void TemaLaborator1::OnWindowResize(int width, int height)
{
	// treat window resize event
}
