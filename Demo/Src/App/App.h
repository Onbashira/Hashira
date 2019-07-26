#pragma once
#include "Hashira.h"
#include "../Scene/DemoScene.h"

class DemoScene;

class App
{
public:

private:
	std::unique_ptr<DemoScene> _scene;

public:
	App();
	~App() {};
	void Run();
private:
};