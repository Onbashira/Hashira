#pragma once

#include "StateNode.h"

namespace K3D {

	class Animator;
	class MMDBoneTree;

	class StateMachineController
	{
		friend class Animator;
	private:
		std::shared_ptr<K3D::StateNode>	_startNode;

		std::shared_ptr<K3D::StateNode>	_exitNode;

		std::weak_ptr<K3D::StateNode>		_currentNode;

		std::map<std::string, std::shared_ptr<K3D::StateNode>> _stateMap;

		std::weak_ptr<K3D::MMDBoneTree>	_skelton;

		unsigned int _animationFrame;


	public:

	private:

		void CCDIKSolver(MMDIKData& IkData, Vector3& skeltonCenterPos, unsigned int iterationNum, unsigned int numBones, unsigned  int numMaxBones);

		void ApplyRecursionMatirixBone(MMDBoneNode* node);

		void Bezeir3(Vector3& sp, Vector3& cp1, Vector3& cp2, Vector3& ep);

	public:
		StateMachineController();

		~StateMachineController();

		void SetModelSkelton(std::weak_ptr<MMDBoneTree> skelton);

		void ResetSkelton();

		void PlayAnimation(unsigned int& time);

		void AttachAnimation(std::string animationName, std::weak_ptr<MotionData> motionData);

		void ConnectingAnimation(std::string from, std::string to, TransitionConditions conditions);

	};
}