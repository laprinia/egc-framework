#pragma once
#include <Component/SimpleScene.h>

class Skyroads : public SimpleScene
{
public:
	Skyroads();
	~Skyroads();
	void Init() override;
private:
	void FrameStart() override;
	void Update(float deltaTimeSeconds) override;
	void FrameEnd() override;
	void HandleCurrentColorPowerup(int currentColorIndex);
	int GenerateRandomLength();
	int GenerateRandomWidth();
	bool CheckEligibleOnRows();
	bool CheckEligibleOnLanes();
	void GeneratePlatforms(int zOffset);
	void GeneratePlatformData();
	void RenderAMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, bool isUI, bool isPlayer,bool isRandomized);
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
	const int minPlatformWidth = 6 + 2;
	const int maxPlatformWidth = 6 + 2;
	const glm::vec3 playerColor = glm::vec3(0.765f, 0.694f, 0.886f);
	const glm::vec3 fuelColor = glm::vec3(0.263f, 0.263f, 0.275f);
	const float intensity = 0.20f;
	const glm::vec3 lightPosition = glm::vec3(0, 80, 0);
	const int materialShininess = 10;
	const float materialKd = 0.5f;
	const float materialKs = 0.5f;
	int randomWidths[rowNumber][laneNumber];
	int randomLengths[rowNumber][laneNumber];
	int randomIndices[rowNumber][laneNumber];
	std::vector<glm::vec3> platformColors;
	glm::vec3 platformCenters[rowNumber][laneNumber];
	float speed;
	float translateZ;
	int currentRow = 0;
	int currentLane = 1;
	glm::vec3 sphereCoordonate;
	float jumpableRowSpace = 7.0f;
	float jumpableLaneSpace = 4.0f;
	bool isFirstPerson = false;
	enum platformColor { orange, blue, red, green, yellow, lime };
	bool isEndGame = false;
	float fuel = 1.0f;
	bool canGetPowerup = true;
	double powerupTimestamp;
	double powerupTimeToWait = 5;
	bool hasOrangePowerup = false;
};

