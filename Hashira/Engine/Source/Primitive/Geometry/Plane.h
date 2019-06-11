#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"

namespace Hashira {

	class GameHeap;

	class Plane : public PrimitiveObject
	{
	public:

		Plane(std::shared_ptr<GameHeap>& heap);

		virtual ~Plane();

		virtual void Update() override;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) override;

		void Initialize();

		void CreateMesh();

		void CreateIndices();

		void CreateDescriptors();

	};
}

