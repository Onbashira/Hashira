#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"
namespace Hashira {
	class Cube : public PrimitiveObject
	{
	public:

	private:
		
	public:

		Cube(std::shared_ptr<GameHeap>& heap);

		virtual ~Cube();

		virtual void Update() override;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) override;
		
		void Initialize();

	private:

		void CreateMesh();

		void CreateIndices();

		void CreateDescriptors();		
	
	private:
	};
}