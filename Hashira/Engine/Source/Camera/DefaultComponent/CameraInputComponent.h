#pragma once
#include "Engine/Source/Component/InputComponent/InputComponent.h"
namespace Hashira {
	class CameraInputComponent :
		public InputComponent
	{
	public:


		CameraInputComponent() {};

		virtual ~CameraInputComponent() {};

		virtual void Update(GameObject* gameObject, InputManager* inputManager) {};
	};

}