#pragma once
#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine//Source/Mesh/ModelMesh.h"
#include "Engine/Source/DrawableComponent/DrawableComponent.h"
#include <vector>

namespace K3D {
	class ModelMesh;

	//ベースのモデルオブジェクト　基本メッシュ情報、描画するためのヒープ、描画するためのパイプライン情報、ゲームオブジェクト属性を持つ
	class ModelObject : public GameObject
	{

	public:

	private:


	protected:

		std::unique_ptr<ModelMesh> _meshData;

	private:

	protected:
		//バンドルリストに継承先特有のドローコールを積む作業を強制
		virtual void RegisterToBundle() = 0;
	public:
		//モデルのアップデート
		virtual void Update() = 0;
		//描画時呼び出し関数の作成を強制
		virtual void Draw() = 0;

		virtual void SetPipelineState(std::weak_ptr<K3D::PipelineStateObject> pipelineState) = 0;

		virtual void SetRootSignature(std::weak_ptr<K3D::RootSignature> rootSignature) = 0;

		virtual void SetCommandList(std::shared_ptr<K3D::CommandList> masterCommandList) = 0;

		virtual void BindingShaderObject() = 0;

		virtual void BindingShaderObjectToBundle() = 0;

		virtual void Draw() = 0;

		virtual void Discard() = 0;

		ModelObject();
		virtual ~ModelObject();
	};
}
