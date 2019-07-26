#pragma once
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"
namespace Hashira {
	class CameraPhysicsComponent :
		public PhysicsComponent
	{
	public:

		CameraPhysicsComponent() {};

		virtual ~CameraPhysicsComponent() {};

		virtual void Update(GameObject* gameObject) {};
	};

}