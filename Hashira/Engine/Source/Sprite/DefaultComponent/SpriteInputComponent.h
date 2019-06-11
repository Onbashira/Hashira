#pragma once
#include "Engine/Source/Component/InputComponent/InputComponent.h"
namespace Hashira {

	class InputManager;
	class GameObject;

	class SpriteInputComponent : public InputComponent
	{
	public:
		
		SpriteInputComponent();
		
		~SpriteInputComponent();

		virtual void Update(GameObject* gameObject, InputManager* inputManager)override;

	};

}