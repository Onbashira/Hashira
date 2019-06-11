#include <map>
#include <iostream>
#include "TransitionConditions.h"
#include "StateNode.h"
#include "Math.h"

K3D::StateNode::StateNode()
{

}


K3D::StateNode::~StateNode()
{
}


void K3D::StateNode::SetMotionData(std::weak_ptr<K3D::MotionData> data)
{
	this->_animation = data;
}

std::weak_ptr<K3D::MotionData> K3D::StateNode::GetMotionData()
{
	return this->_animation;
}

void K3D::StateNode::SetTransitionConditions(const std::pair<std::string, std::weak_ptr<K3D::StateNode>>& destination, K3D::TransitionConditions consitions)
{
	if (this->_transitionMap.find(destination.first) != this->_transitionMap.end()) {
		this->_transitionMap.find(destination.first)->second.second = consitions;
		this->_transitionMap.find(destination.first)->second.first = destination.second;
	}
}


