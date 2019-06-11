#include <memory>
#include "Animator.h"
#include "TransitionConditions.h"
#include "StateMachineController.h"
#include "../Model/MMDModel/MMDModel.h"
#include "VMDLoader.h"
#include "MotionManager.h"

K3D::Animator::Animator() :_animationFrame(0)
{
}


K3D::Animator::~Animator()
{
}


void K3D::Animator::SetModelSkelton(std::weak_ptr<K3D::MMDBoneTree> skelton)
{
	_statemachineController.SetModelSkelton(skelton);
}

void K3D::Animator::AttachAnimation(std::string animationName, std::string animationFilePath)
{
	_statemachineController.AttachAnimation(animationName, K3D::MotionManager::LoadMotion(animationFilePath).lock());
}

void K3D::Animator::PlayAnimation(std::string animation)
{
	_statemachineController.PlayAnimation(_animationFrame);
}

void K3D::Animator::SetTransitionCondition(std::string from, std::string to, K3D::TransitionConditions newCondition)
{
	_statemachineController.ConnectingAnimation(from, to, newCondition);
}

void K3D::Animator::ResetSkelton()
{
	_statemachineController.ResetSkelton();
}


