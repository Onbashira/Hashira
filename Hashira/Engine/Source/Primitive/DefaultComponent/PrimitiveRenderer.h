#pragma once

#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"

namespace Hashira {
	class PrimitiveRenderer : public GraphicsComponent
	{

	public:

	private:

	public:

		PrimitiveRenderer();

		virtual ~PrimitiveRenderer();

		virtual void Draw(std::shared_ptr<CommandList>& cmdList,GameObject* ojcet, GeometryState* geometryState,ModelMesh* mesh)override;

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState)override;

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature)override;


	};

}