#include "TransitionConditions.h"



K3D::TransitionConditions::TransitionConditions()
{
}

K3D::TransitionConditions::TransitionConditions(std::function<bool()> condition, bool hasExitTime, float exitTime, bool isLoop) :
	_hasExitTime(hasExitTime),_isLoop(isLoop),_exitTime(exitTime),_conditionFunction(condition)
{

}


K3D::TransitionConditions::~TransitionConditions()
{
}


void K3D::TransitionConditions::EnableLoop()
{
	_isLoop = true;
}

void K3D::TransitionConditions::DisableLoop()
{
	_isLoop = false;
}

void K3D::TransitionConditions::EnableHasExitTime()
{
	_hasExitTime = true;
}

void K3D::TransitionConditions::DisableHasExitTime()
{
	_hasExitTime = false;

}

void K3D::TransitionConditions::SetExitTime(float exitTime)
{
	_exitTime = exitTime;
}

float K3D::TransitionConditions::GetExitTime()
{
	return _exitTime;
}

bool K3D::TransitionConditions::CheckCondition()
{
	return _conditionFunction();
}

void K3D::TransitionConditions::ResetCondiiton(std::function<bool()> newCondition)
{
	_conditionFunction = newCondition;
}
