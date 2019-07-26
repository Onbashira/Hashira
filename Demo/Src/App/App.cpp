#include "App.h"
using namespace Hashira;

App::App()
{

}


void App::Run()
{
	Framework::SetUp();
	auto& ref = Hashira::Framework::GetInstance();
	ref.SetBackBufferNum(3);
	ref.SetWindowSize(1280, 720);
	ref.SetWindowName(L"DEMO");
	ref.Initialize();

	ref.TimerStart();
	this->_scene->Initialize();
	while (ref.LoopMessage() != 0)
	{
		ref.Input().InputUpdate();
		this->_scene->SceneBegin();
		this->_scene->Update();
		this->_scene->Rendering();
		this->_scene->SceneEnd();
		ref.Tick();
	}
	ref.TimerStop();
	ref.Terminate();
}
