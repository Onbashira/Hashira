#pragma once
namespace Hashira {

	class GameObject;
	class GeometryState;
	class PipelineStateObject;
	class RootSignature;
	class CommandList;
	class ModelMesh;

	//描画ベースコンポネント

	class GraphicsComponent 
	{
	public:

	private:

	public:

		GraphicsComponent() {};

		virtual ~GraphicsComponent() {};

		virtual void Draw(std::shared_ptr<CommandList>& cmdList,GameObject* object,GeometryState* geometryState,ModelMesh* mesh) = 0;

		virtual void SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState) = 0;

		virtual void SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature) = 0;

	private:

	};
}