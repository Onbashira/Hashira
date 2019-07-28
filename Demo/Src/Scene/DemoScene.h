#pragma once
#include "Hashira.h"

class DemoScene : public Hashira::Scene
{

public:

private:

	Hashira::VertexBuffer _planeVert;
	Hashira::IndexBuffer _indexBuffer;
	Hashira::DescriptorSet _descriptorSets;
	std::shared_ptr<Hashira::RootSignature> _renderSignature;
	std::shared_ptr<Hashira::PipelineStateObject> _pso;
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