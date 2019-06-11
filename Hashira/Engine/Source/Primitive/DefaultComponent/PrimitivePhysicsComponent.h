#pragma once
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"
namespace Hashira {
	class GameObject;

	class PrimitivePhysicsComponent :public PhysicsComponent
	{
	public:
		PrimitivePhysicsComponent();
		virtual ~PrimitivePhysicsComponent();

		virtual void Update(GameObject* gameObject)override;

	};

}