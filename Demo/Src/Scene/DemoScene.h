#pragma once
#include "Hashira.h"
#include "Engine/Source/ShaderObject/Shader.h"
class DemoScene : public Hashira::Scene
{

public:

private:

	Hashira::VertexBuffer _planeVert;
	Hashira::IndexBuffer _indexBuffer;
	Hashira::DescriptorSet _descriptorSets;
	std::unique_ptr<Hashira::RootSignature> _rootSignature;
	std::unique_ptr<Hashira::GraphicsPipelineState> _pso;
	Hashira::Shader _vs;
	Hashira::Shader _ps;

public:

	DemoScene();

	~DemoScene();

	HRESULT Initialize();


	virtual void Update();

	virtual void Rendering();

	void Discard();

private:

	HRESULT PSOInitialize();

	HRESULT PlaneInitialize();

	HRESULT RootSignatureInitialize();

	void DescriptorSetInitialize();

};