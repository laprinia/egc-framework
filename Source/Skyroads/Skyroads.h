#pragma once
#include <Component/SimpleScene.h>

class Skyroads: public SimpleScene
{	
public:
	Skyroads();
	~Skyroads();
	void Init() override;
private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	int GenerateRandomLength();
	int GenerateRandomWidth();
	bool CheckEligibleOnRows();
	bool CheckEligibleOnLanes();
	void GeneratePlatforms(int zOffset);
	void GeneratePlatformData();
	void RenderAMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	const static int laneNumber = 4;
	const static int rowNumber = 5;
	const int minPlatformLength = 5;
	const int maxPlatformLength = 20;
	const int minPlatformWidth = 6+2;
	const int maxPlatformWidth = 6+7;
	const glm::vec3 playerColor = glm::vec3(0.989f, 0.357f, 0.357f);
	const float intensity = 0.20f;
	const glm::vec3 lightPosition = glm::vec3(0, 80, 0);
	const int materialShininess = 10;
	const float materialKd = 0.5f;
	const float materialKs = 0.5f;
	int randomWidths[rowNumber][laneNumber];
	int randomLengths[rowNumber][laneNumber];
	std::vector<int> randomIndices;
	std::vector<glm::vec3> platformColors;
	glm::vec3 platformCenters[rowNumber][laneNumber];
	float speed;
	float translateZ;
	int currentRow = 0;
	int currentLane = 1;
	glm::vec3 sphereCoordonate;
	float jumpableRowSpace = 6.0f;
	float jumpableLaneSpace = 4.0f;

};

