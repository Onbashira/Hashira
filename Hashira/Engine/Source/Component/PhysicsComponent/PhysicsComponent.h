#pragma once
namespace Hashira {
	
	class GameObject;
	
	class PhysicsComponent
	{
	public:

	private:

	public:

		PhysicsComponent() {};

		virtual ~PhysicsComponent() {};

		virtual void Update(GameObject* gameObject) = 0;
	private:
	};

}