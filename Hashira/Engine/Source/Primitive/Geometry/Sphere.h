#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"

namespace Hashira {
	class Sphere : public PrimitiveObject
	{
	public:
		
		Sphere(std::shared_ptr<GameHeap>& heap);

		~Sphere();

		virtual void Update() override;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) override;

		void Initialize();

	private:

		void CreateMesh();

		void CreateIndices();

		void CreateDescriptors();
	};
}