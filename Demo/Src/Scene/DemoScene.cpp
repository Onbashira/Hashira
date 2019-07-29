#include "DemoScene.h"
#include "Engine/Source/ShaderObject/Shader.h"
using namespace Hashira;

DemoScene::DemoScene() : Hashira::Scene(10000,256,256,16)
{

}

DemoScene::~DemoScene()
{
}

HRESULT DemoScene::Initialize() 
{


	
	if (FAILED(PlaneInitialize()))
	{
		return E_FAIL;
	}
	if (FAILED(RootSignatureInitialize()))
	{
		return E_FAIL;
	}
	if(FAILED(PSOInitialize()))
	{
		return E_FAIL;
	}

	DescriptorSetInitialize();

	return S_OK;
}

void DemoScene::Update()
{
	Hashira::SceneConstant sc;
	sc.resolution = Hashira::Vector2(1280.0f, 720.0f);
	sc.time = Hashira::Framework::GetInstance().Time().TotalTime();
	_sceneConstant.Update(&sc,sizeof(Hashira::SceneConstant) , 0);

}

void DemoScene::Rendering()
{
	//SceneRendering

	std::shared_ptr<Hashira::CommandList> _cmdList;
	this->_renderContext->CreateCommandList(D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdList);
	_renderContext->ResetCommandList(_cmdList);

	_cmdList->RSSetViewports(this->_mainCamera->GetViewportNum(), _mainCamera->GetViewportArray().data());
	_cmdList->GetCommandList()->SetPipelineState(_pso->GetPSO().Get());	
	_renderContext->GetSwapChain()->SetRenderTarget(_cmdList);
	_cmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	_cmdList->SetGraphcisRootSignatureAndDescriptors(_rootSignature.get() , &_descriptorSets);
	_cmdList->IASetVertexBuffers(0,1,&_planeVert.GetView());
	_cmdList->DrawInstanced(4, 1, 0, 0);
	_cmdList->CloseCommandList();
	_renderContext->PushBackCmdList(_cmdList);

}

void DemoScene::Discard()
{

}

HRESULT DemoScene::PSOInitialize()
{
	HRESULT hr{};



	//CreatePso

	D3D12_INPUT_ELEMENT_DESC elementDescs[] = {
	{ "INDEX", 0, DXGI_FORMAT_R32_UINT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	//Shader Comppile
	Hashira::GraphicsPipelineStateDesc desc{};
	desc.inputLayout.numElements = _countof(elementDescs);
	desc.inputLayout.pElements = elementDescs;

	desc.rootSignature = _rootSignature.get();

	desc.vs = &_vs;
	desc.ps = &_ps;

	desc.blendDesc.isAlphaToCoverageEnable = false;
	desc.blendDesc.isIndependentBlend = false;
	desc.blendDesc.sampleMask = UINT_MAX;
	desc.blendDesc.rtDesc[0] = Hashira::DefaultRenderTargetBlendNone();

	desc.depthStencilDesc = DefaultDepthStateDisableDisable();

	desc.rasterizerDesc = DefaultRasterizerStateStandard();
	desc.rasterizerDesc.cullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;

	desc.primitiveTopology = D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
	desc.numRTVs = 1;
	desc.rtvFormats[0] = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.dsvFormat = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	desc.multiSamplerCount = 1;
	

	this->_pso = std::make_unique<Hashira::GraphicsPipelineState>();
	hr = this->_pso->Initialize(_renderContext->GetRenderingDevice()->GetD3D12Device(), desc);
	if (FAILED(hr)) {
		return hr;
	}


	CHECK_RESULT(hr);
	return hr;
}

HRESULT DemoScene::PlaneInitialize()
{

	std::vector<int> vertIndex(4);
	for (int i = 0; i < 4; ++i)
	{
		vertIndex[i] = i;
	}

	return this->_planeVert.Initialize(sizeof(int) * 4, sizeof(int), vertIndex.data());
	
}

HRESULT DemoScene::RootSignatureInitialize()
{

	//Compile Shader


	auto hr = _vs.CompileShader(Hashira::Shader::Type::SHADER_TYPE_VERTEX,
		"./Src/Shader/RayMarchingShader.hlsl", "VS_Main", "vs_5_0");
	if (FAILED(hr)) {
		return hr;
	}
	hr = _ps.CompileShader(Hashira::Shader::Type::SHADER_TYPE_VERTEX,
		"./Src/Shader/RayMarchingShader.hlsl", "PS_Main", "ps_5_0");
	if (FAILED(hr)) {
		return hr;
	}

	Hashira::RootSignatureDesc sigDesc;
	Hashira::RootParameter param;

	param.registerIndex = 0;
	param.shaderVisibility = ShaderVisibility::All;
	param.type = RootParameterType::ConstantBuffer;

	sigDesc.flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	sigDesc.numParameters = 1;
	sigDesc.pParameters = &param;
	_rootSignature = std::make_unique<RootSignature>();
	 hr = this->_rootSignature->InitializeFromDesc(this->_renderingDevice->GetD3D12Device(), sigDesc);

	return hr;
}

void DemoScene::DescriptorSetInitialize()
{
}
