#pragma once
#include "Engine/Source/Component/GameObject/GameObject.h"

namespace Hashira {

	class ModelMesh;
	class GameHeap;

	struct PrimitiveVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texcoord;
	};

	class PrimitiveObject :public GameObject
	{
	public:
	protected:
		std::unique_ptr<ModelMesh> _modelMesh;
	private:


	public:

		PrimitiveObject(GraphicsComponent* graphicsComponent, InputComponent* inputComponent, PhysicsComponent* physicsComponent, std::shared_ptr<GameHeap>& _gameHeap);

		virtual ~PrimitiveObject();
	private:

		PrimitiveObject();

	};

}