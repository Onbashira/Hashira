#pragma once
#include "Engine/Source/Component/InputComponent/InputComponent.h"
namespace Hashira {

	class InputManager;
	class GameObject;

	class PrimitiveInputComponent : public InputComponent
	{
	public:
		
		PrimitiveInputComponent();
		
		~PrimitiveInputComponent();

		virtual void Update(GameObject* gameObject, InputManager* inputManager)override;

	};

}