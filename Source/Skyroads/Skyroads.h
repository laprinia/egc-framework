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
	void GenerateRowOfPlatforms(int zOffset);
	void OnInputUpdate(float deltaTime, int mods) override;
	void OnKeyPress(int key, int mods) override;
	void OnKeyRelease(int key, int mods) override;
	void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
	void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
	void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
	void OnWindowResize(int width, int height) override;
	const int laneNumber = 3;
	const int rowNumber = 5;
	const int minPlatformLength = 5;
	const int maxPlatformLength = 20;
	const int minPlatformWidth = 7;
	const int maxPlatformWidth = 10;
	std::vector<int> randomLengths;
	std::vector<int> randomWidths;

};
