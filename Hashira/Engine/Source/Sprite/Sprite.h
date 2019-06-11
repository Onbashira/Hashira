#pragma once
#include "Engine/Source/Primitive/Object/PrimitiveObject.h"

namespace Hashira {

	class CommandList;
	class GameHeap;
	class ConstantBuffer;
	class TextureObject;

	class Sprite :public PrimitiveObject
	{
	public:
		struct SpriteMaterial {
			Vector4 albedo;
		};
	private:

		std::shared_ptr<ConstantBuffer> _materialBuffer;

		std::shared_ptr<TextureObject> _texture;

	public:

		Sprite(std::shared_ptr<GameHeap>& heap);

		virtual ~Sprite();

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