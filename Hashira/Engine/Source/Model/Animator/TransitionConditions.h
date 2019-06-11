#pragma once
#include <functional>

namespace K3D {
	class TransitionConditions {
	private:

		bool _hasExitTime;

		float _exitTime;

		bool _isLoop;

		std::function<bool()> _conditionFunction;

	public:

	private:

	public:

		TransitionConditions();

		TransitionConditions(std::function<bool()> condition, bool hasExitTime = false, float  exitTime = 0.0f, bool isLoop = true);

		~TransitionConditions();

		void EnableLoop();

		void DisableLoop();

		void EnableHasExitTime();

		void DisableHasExitTime();

		void SetExitTime(float exitTime = 1.0f);

		float GetExitTime();

		virtual bool CheckCondition();

		void ResetCondiiton(std::function<bool()> newCondition);

	};
}