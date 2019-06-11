#pragma once

#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"

namespace Hashira {
	class SpriteRenderer : public GraphicsComponent
	{

	public:

	private:

	public:

		SpriteRenderer();

		virtual ~SpriteRenderer();

		virtual void Draw(std::shared_ptr<CommandList>& cmdList,GameObject* ojcet, GeometryState* geometryState,ModelMesh* mesh)override;

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState)override;

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature)override;


	};

}