#pragma once
#include <map>
#include <vector>
#include <string>
#include <functional>
#include "VMD.h"


namespace K3D {

	class StateMachineController;
	class TransitionConditions;

	class StateNode
	{
		friend class StateMachineController;
	private:
		//�J�ڐ於�O�A�J�ڏ���
		std::map<std::string, std::pair<std::weak_ptr<K3D::StateNode>, K3D::TransitionConditions>>	_transitionMap;
		//���̃X�e�[�g�̖��O
		std::string															_stateName;
		//���[�V�����f�[�^�ւ̎�Q��
		std::weak_ptr<K3D::MotionData>											_animation;
	public:

	private:

	public:
		StateNode();

		~StateNode();

		void SetMotionData(std::weak_ptr<K3D::MotionData> data);

		std::weak_ptr<K3D::MotionData> GetMotionData();

		void SetTransitionConditions(const std::pair<std::string, std::weak_ptr<K3D::StateNode>>& destination, K3D::TransitionConditions consitions);

	};
}