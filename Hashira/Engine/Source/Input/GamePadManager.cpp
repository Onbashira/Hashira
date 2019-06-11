#include "GamePadManager.h"
#include "GamePad.h"
#include "Engine/Source/Utility/Utility.h"
constexpr int MaxConectingCount = 4;

void Hashira::GamePadManager::AllSetUp()
{
	for (int i = 0; i < _maxConectionCount;i++) {
		this->_gamePads.push_back(std::move(std::make_shared<GamePad>()));
		this->_gamePads[i]->SetUp(i);
	}
}

void Hashira::GamePadManager::Update()
{
	for (size_t i = 0; i < _gamePads.size(); i++) {
		this->_gamePads[i]->Update();
	}
}

std::shared_ptr<Hashira::GamePad> Hashira::GamePadManager::GetPad(int padID)
{
	return _gamePads[::Clamp(padID, 0, MaxConectingCount-1)];
}

void Hashira::GamePadManager::Initialize(int maxConectionCount)
{
	this->_maxConectionCount = ::Clamp(maxConectionCount,0, MaxConectingCount);
	AllSetUp();
}

void Hashira::GamePadManager::Discard()
{
	for (int i = 0; i < _maxConectionCount; i++) {
		this->_gamePads[i].reset();
	}
	
}

Hashira::GamePadManager::GamePadManager() : _maxConectionCount(0)
{
}

Hashira::GamePadManager::~GamePadManager()
{
	Discard();
}
