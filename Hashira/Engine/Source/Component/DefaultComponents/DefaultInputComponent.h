#pragma once
#include "Engine/Source/Component/InputComponent/InputComponent.h"


namespace Hashira {
	class DefaultInputComponent : public InputComponent
	{
	public:
		DefaultInputComponent();
		virtual ~DefaultInputComponent();

		virtual void Update(GameObject* gameObject, InputManager* inputManager)override {};

	};

}