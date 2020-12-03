#include "Laborator3_Vis2D.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3_Vis2D::Laborator3_Vis2D()
{
}

Laborator3_Vis2D::~Laborator3_Vis2D()
{
}

void Laborator3_Vis2D::Init()
{
	auto camera = GetSceneCamera();
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);

	logicSpace.x = 0;		// logic x
	logicSpace.y = 0;		// logic y
	logicSpace.width = 4;	// logic width
	logicSpace.height = 4;	// logic height

	ftx, fty = 0;
	fsx = 1; fsy = 1;

	glm::vec3 corner = glm::vec3(0.001, 0.001, 0);
	length = 0.99f;

	Mesh* square1 = Object2D::CreateSquare("square1", corner, length, glm::vec3(1, 0, 0));
	AddMeshToList(square1);
}

// 2D visualization matrix
glm::mat3 Laborator3_Vis2D::VisualizationTransf2D(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty;
	
	sx = viewSpace.width / logicSpace.width;
	sy =viewSpace.height / logicSpace.height;
	tx = viewSpace.x - sx * logicSpace.x;
	ty = viewSpace.y - sy * logicSpace.y;

	return glm::transpose(glm::mat3(
		sx, 0.0f, tx,
		0.0f, sy, ty,
		0.0f, 0.0f, 1.0f));
}

// uniform 2D visualization matrix (same scale factor on x and y axes)
glm::mat3 Laborator3_Vis2D::VisualizationTransf2DUnif(const LogicSpace & logicSpace, const ViewportSpace & viewSpace)
{
	float sx, sy, tx, ty, smin;
	sx = viewSpace.width / logicSpace.width;
	sy = viewSpace.height / logicSpace.height;
	if (sx < sy)
		smin = sx;
	else
		smin = sy;
	tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
	ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;

	return glm::transpose(glm::mat3(
		smin, 0.0f, tx,
		0.0f, smin, ty,
		0.0f, 0.0f, 1.0f));
}

void Laborator3_Vis2D::SetViewportArea(const ViewportSpace & viewSpace, glm::vec3 colorColor, bool clear)
{
	glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	glEnable(GL_SCISSOR_TEST);
	glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);

	GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
	GetSceneCamera()->Update();
}

void Laborator3_Vis2D::FrameStart()
{
	// Clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

}

void Laborator3_Vis2D::Update(float deltaTimeSeconds)
{

	glm::ivec2 resolution = window->GetResolution();

	// Sets the screen area where to draw - the left half of the window
	viewSpace = ViewportSpace(0, 0, resolution.x / 2, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0), true);
	float cx = viewSpace.width / 2 + viewSpace.x;
	float cy = viewSpace.height / 2 + viewSpace.y;
	// Compute the 2D visualization matrix
	visMatrix = glm::mat3(1);
	
	visMatrix *= Transform2D::Translate(cx, cy);
	visMatrix *= MatrixModifier();
	visMatrix *= Transform2D::Translate(-cx, -cy);
	visMatrix *= VisualizationTransf2D(logicSpace, viewSpace);
	
	

	DrawScene(visMatrix);

	// The viewport is now the right half of the window

	viewSpace = ViewportSpace(resolution.x / 2, 0, resolution.x / 2, resolution.y);
	SetViewportArea(viewSpace, glm::vec3(0.5f), true);

	// Compute uniform 2D visualization matrix
	 cx = viewSpace.width / 2 + viewSpace.x;
	 cy = viewSpace.height / 2 + viewSpace.y;
	visMatrix = glm::mat3(1);

	visMatrix *= Transform2D::Translate(cx, cy);
	visMatrix *= MatrixModifier();
	visMatrix *= Transform2D::Translate(-cx, -cy);
	visMatrix *= VisualizationTransf2DUnif(logicSpace,viewSpace);
	
	

	DrawScene(visMatrix);
}
glm::mat3 Laborator3_Vis2D::MatrixModifier() {
	
	return glm::transpose(glm::mat3(
		fsx, 0.0f, ftx,
		0.0f, fsy, fty,
		0.0f, 0.0f, 1.0f));
}

void Laborator3_Vis2D::FrameEnd()
{

}

void Laborator3_Vis2D::DrawScene(glm::mat3 visMatrix)
{
	modelMatrix = visMatrix * Transform2D::Translate(0, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(3, 0);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(1.5, 1.5);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(0, 3);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);

	modelMatrix = visMatrix * Transform2D::Translate(3, 3);
	RenderMesh2D(meshes["square1"], shaders["VertexColor"], modelMatrix);
}

void Laborator3_Vis2D::OnInputUpdate(float deltaTime, int mods)
{
	float offset = deltaTime * 50;
	float zoomOffset = deltaTime * 2;
	
	if (window->KeyHold(GLFW_KEY_W)) {
		fty -= offset;
	}else if (window->KeyHold(GLFW_KEY_A)) {
		ftx += offset;
	}else if (window->KeyHold(GLFW_KEY_S)) {
		fty += offset;
	}else if (window->KeyHold(GLFW_KEY_D)) {
		ftx -= offset;
	}else if (window->KeyHold(GLFW_KEY_Z)) {
		fsy += zoomOffset; fsx += zoomOffset;

	}else if (window->KeyHold(GLFW_KEY_X)) {
		fsy -= zoomOffset; fsx -= zoomOffset;
	}
}


void Laborator3_Vis2D::OnKeyPress(int key, int mods)
{

}

void Laborator3_Vis2D::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3_Vis2D::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3_Vis2D::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Laborator3_Vis2D::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3_Vis2D::OnMouseScroll(int mouseX, int mouseY, int zoomOffsetX, int zoomOffsetY)
{
}