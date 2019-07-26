#pragma once
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
namespace Hashira {
	class Commandist;

	class CameraGraphicsComponent :
		public GraphicsComponent
	{
	public:
		CameraGraphicsComponent() {};

		virtual ~CameraGraphicsComponent() {};

		virtual void Draw(std::shared_ptr<CommandList>& cmdList, GameObject* object, GeometryState* geometryState, ModelMesh* mesh) {};

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState) {};

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature) {};

	};

}