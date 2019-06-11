#include "InputManager.h"


Hashira::InputManager::InputManager()  :
	_mousePos({})
{
}


Hashira::InputManager::~InputManager()
{
}


void Hashira::InputManager::InputUpdate()
{
	CopyMemory(_preKey,_key,sizeof(_preKey));
	if (!GetKeyboardState(&_key[0])) {
		MessageBox(nullptr, L"ƒL[î•ñ‚Ìæ“¾‚É¸”s", L"ERROR", MB_OK);
		return ;
	}
	POINT pos;
	GetCursorPos(&pos);
	ScreenToClient(_forcusWindowHandle, &pos);
#ifdef _DEBUG
	//std::cout <<pos.x << std::endl;
	//std::cout <<pos.y << std::endl;
#endif // _DEBUG
	_mousePos.x = float(pos.x);
	_mousePos.x = float(pos.y);

	_gamePadManager.Update();

}

bool Hashira::InputManager::IsTriggerDown(VIRTUAL_KEY_STATE code)
{
	if (!((_preKey[code] & (0x80)) == 0x80) && ((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool Hashira::InputManager::IsTriggerUp(VIRTUAL_KEY_STATE code)
{
	if (((_preKey[code] & (0x80)) == 0x80) && !((_key[code] & (0x80)) == 0x80)) {
		return true;
	}
	return false;
}

bool Hashira::InputManager::IsToggle(VIRTUAL_KEY_STATE code)
{
	if ((_key[code] & (0x01f)) == (0x01)) {
		return true;
	};
	return false;
}

Hashira::Vector2 Hashira::InputManager::GetMousePos()
{
	return _mousePos;
}


bool Hashira::InputManager::IsDown(Hashira::VIRTUAL_KEY_STATE code)
{

	if (_key[code] & 0x80) {
		return true;
	}
	return false;
}

void Hashira::InputManager::Discard()
{
	this->_forcusWindowHandle = nullptr;
}

void Hashira::InputManager::SetFocusWindow(HWND forcusWindow)
{
	_forcusWindowHandle = forcusWindow;
}

void Hashira::InputManager::GamePadInitialize(int cotrollerNum)
{
	_gamePadManager.Initialize(cotrollerNum);
}

Hashira::GamePadManager & Hashira::InputManager::GetGamePadManager()
{
	return _gamePadManager;
}

std::shared_ptr<Hashira::GamePad> Hashira::InputManager::GetGamePad(int controllerID)
{
	return _gamePadManager.GetPad(controllerID);
}
