#include "Framework.h"
#include "Engine/Source/GraphicsManager/GraphicsContextManager.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Texture/TextureManager.h"

Hashira::Framework::Framework() :
	_windowWidth(640), _windowHeight(480), _useWarpDevice(false), _backBufferNum(3)
{

}

Hashira::Framework::~Framework()
{

}


HRESULT Hashira::Framework::Initialize()
{

	CHECK_RESULT(_instance->InitWindow());
	CHECK_RESULT(_instance->InitD3D12());

	return S_OK;
}

void Hashira::Framework::Terminate()
{
	_instance->TermD3D();
	_instance->TermWindow();
}

std::shared_ptr<Hashira::D3D12Device> & Hashira::Framework::GetDevice()
{
	return _instance->_device;
}

Hashira::Factory & Hashira::Framework::GetFactory()
{
	return _instance->_factory;
}

Hashira::Window & Hashira::Framework::GetWindow()
{
	return _instance->_window;
}

Hashira::RenderingManager & Hashira::Framework::GetRenderingManagre()
{
	return _instance->_renderingManager;
}

Hashira::Timer & Hashira::Framework::Time()
{
	return _instance->_timer;
}

void Hashira::Framework::FlipScreen()
{
	return _instance->_renderingManager.FlipScreen();

}

void Hashira::Framework::ClearScreen(std::weak_ptr<CommandList> list)
{

	return _instance->_renderingManager.ClearScreen(list);

}

void Hashira::Framework::SetWindowName(std::wstring name)
{
	_instance->_appClassName = name;
}

void Hashira::Framework::SetWindowSize(unsigned int width, unsigned int height)
{
	_instance->_windowHeight = height;
	_instance->_windowWidth = width;
}

void Hashira::Framework::GetWindowSize(unsigned int & width, unsigned int & height)
{
	width = _windowWidth;
	height = _windowHeight;
}

void Hashira::Framework::SetBackBufferNum(unsigned int backBufferNum)
{
	_instance->_backBufferNum = backBufferNum;
}

void Hashira::Framework::TimerStart()
{
	_instance->_timer.Start();
}

void Hashira::Framework::TimerStop()
{
	_instance->_timer.Stop();

}

void Hashira::Framework::Tick()
{
	_instance->_timer.Tick();

}

float Hashira::Framework::DeltaTime()
{
	return 	_instance->_timer.DeltaTime();
}

Hashira::InputManager& Hashira::Framework::Input()
{
	return _instance->_inputManager;
}

Hashira::AudioManager & Hashira::Framework::AudioManager()
{
	return _instance->_audioManager;
}

int Hashira::Framework::LoopMessage()
{
	MSG msg = {};
	while (true) {
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				return -1;
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return 0;
		}
	}
}

HRESULT Hashira::Framework::InitWindow()
{
	return _window.Create(_appClassName, _windowWidth, _windowHeight);
}

HRESULT Hashira::Framework::InitD3D12()
{
	CHECK_RESULT(InitFactory());

	CHECK_RESULT(InitDevice());

	CHECK_RESULT(InitRenderingManager());

	CHECK_RESULT(InitInputManager());

	CHECK_RESULT(InitAudioManager());


	return S_OK;
}

HRESULT Hashira::Framework::InitDevice()
{
	HRESULT ret = {};
	_device = std::make_shared<D3D12Device>();
	ret = _device->Initialize(&_factory, _useWarpDevice);

	if (FAILED(ret)) {

		Util::Comment(L"D3D12Device‚Ì‰Šú‰»‚ÉŽ¸”s");
		return ret;
	}

	return ret;
}
HRESULT Hashira::Framework::InitFactory()
{
	return _factory.Initialize();
}


HRESULT Hashira::Framework::InitRenderingManager()
{
	return _renderingManager.Initialize(_device, &_factory, &_window, _windowWidth, _windowHeight, _backBufferNum);
}

HRESULT Hashira::Framework::InitInputManager()
{
	_inputManager.GamePadInitialize();
	_inputManager.SetFocusWindow(_window.GetWindowHandle());
	return S_OK;
}

HRESULT Hashira::Framework::InitAudioManager()
{
	_audioManager.InitializeXAudio2();
	_audioManager.StartSoundEngine();
	return S_OK;
}

HRESULT Hashira::Framework::InitTextureManager()
{
	TextureManager::GetInstance().SetRenderContext(_instance->_renderingManager.CreateRenderContext());
	return S_OK;
}

void Hashira::Framework::TermWindow()
{
	_inputManager.Discard();
	_audioManager.StopSoundEngine();
	_audioManager.Discard();
	TextureManager::GetInstance().Discard();
	_window.Discard();
}

void Hashira::Framework::TermD3D()
{
	_renderingManager.Term();
	this->_timer.Stop();
	this->_factory.Discard();
	this->_device.reset();
}