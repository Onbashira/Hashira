#include "DemoScene.h"

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
}

void DemoScene::Discard()
{
}

HRESULT DemoScene::PSOInitialize()
{

	Microsoft::WRL::ComPtr<ID3DBlob> vs;
	Microsoft::WRL::ComPtr<ID3DBlob> gs;
	Microsoft::WRL::ComPtr<ID3DBlob> ds;
	Microsoft::WRL::ComPtr<ID3DBlob> hs;
	Microsoft::WRL::ComPtr<ID3DBlob> ps;
	Microsoft::WRL::ComPtr<ID3DBlob> error;

#if defined(_DEBUG)
	//グラフィックデバッグツールによるシェーダーのデバッグの有効化処理
	UINT compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;

#else
	UINT compileFlag = 0;
#endif

	HLSLIncluder includes("./Engine/Shader/");

	if (FAILED(D3DCompileFromFile(L"./Demo/Src/Shader/RaymarchingShader.hlsl", nullptr, &includes, "VS_Main", "vs_5_0", compileFlag, 0, &vs, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	if (FAILED(D3DCompileFromFile(L"./Demo/Src/Shader/RaymarchingShader.hlsl", nullptr, &includes, "PS_Main", "ps_5_0", compileFlag, 0, &ps, &error))) {
		OutputDebugStringA((char*)error->GetBufferPointer());
		return E_FAIL;
	}
	if (error != nullptr) {
		OutputDebugStringA((char*)error->GetBufferPointer());
	}

	//頂点入力レイアウトの定義
	D3D12_INPUT_ELEMENT_DESC inputElementDesc[] = {
		{ "INDEX",  0, DXGI_FORMAT::DXGI_FORMAT_R32_UINT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	};

	//ラスタライザステートの設定
	D3D12_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
	rasterizerDesc.CullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
	rasterizerDesc.FrontCounterClockwise = FALSE;
	rasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	rasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	rasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	rasterizerDesc.DepthClipEnable = TRUE;
	rasterizerDesc.MultisampleEnable = FALSE;
	rasterizerDesc.AntialiasedLineEnable = FALSE;
	rasterizerDesc.ForcedSampleCount = 0;
	rasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;

	//レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC descRTBS = {};

	descRTBS.BlendEnable = FALSE;
	descRTBS.LogicOpEnable = FALSE;
	descRTBS.SrcBlend = D3D12_BLEND::D3D12_BLEND_SRC_ALPHA;
	descRTBS.DestBlend = D3D12_BLEND::D3D12_BLEND_INV_SRC_ALPHA;
	descRTBS.BlendOp = D3D12_BLEND_OP_ADD;
	descRTBS.SrcBlendAlpha = D3D12_BLEND::D3D12_BLEND_ONE;
	descRTBS.DestBlendAlpha = D3D12_BLEND::D3D12_BLEND_ZERO;
	descRTBS.BlendOpAlpha = D3D12_BLEND_OP::D3D12_BLEND_OP_ADD;
	descRTBS.LogicOp = D3D12_LOGIC_OP_CLEAR;
	descRTBS.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	//ブレンドステートの設定
	D3D12_BLEND_DESC descBS;
	descBS.AlphaToCoverageEnable = FALSE;
	descBS.IndependentBlendEnable = FALSE;
	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++) {
		descBS.RenderTarget[i] = descRTBS;
	}
	//パイプラインステートオブジェクト
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout.pInputElementDescs = inputElementDesc;
	psoDesc.InputLayout.NumElements = _countof(inputElementDesc);
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = vs->GetBufferPointer();
		shaderBytecode.BytecodeLength = vs->GetBufferSize();
		psoDesc.VS = shaderBytecode;
	}
	{
		D3D12_SHADER_BYTECODE shaderBytecode;
		shaderBytecode.pShaderBytecode = ps->GetBufferPointer();
		shaderBytecode.BytecodeLength = ps->GetBufferSize();
		psoDesc.PS = shaderBytecode;
	}

	psoDesc.RasterizerState = rasterizerDesc;
	psoDesc.BlendState = descBS;
	psoDesc.SampleMask = UINT_MAX;											//mask
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	psoDesc.SampleDesc.Quality = 0;
	//デプスステンシルステートの設定
	psoDesc.DepthStencilState.DepthEnable = FALSE;								//深度テストあり
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;							//ステンシルテストなし
	psoDesc.DepthStencilState.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	psoDesc.DepthStencilState.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	psoDesc.DepthStencilState.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	psoDesc.DepthStencilState.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	psoDesc.DepthStencilState.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	//パイプラインステートオブジェクトの生成
	_pso = std::make_shared<PipelineStateObject>();

	auto hr = _pso->Initialize("Raymarch", psoDesc, _renderSignature);
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

	Hashira::RootSignatureDesc sigDesc;
	Hashira::RootParameter param;

	param.registerIndex = 0;
	param.shaderVisibility = ShaderVisibility::All;
	param.type = RootParameterType::ConstantBuffer;

	sigDesc.flags = D3D12_ROOT_SIGNATURE_FLAGS::D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	sigDesc.numParameters = 1;
	sigDesc.pParameters = &param;
	_renderSignature = std::make_shared<RootSignature>();
	auto hr = this->_renderSignature->InitializeFromDesc(this->_renderingDevice->GetD3D12Device(), sigDesc);

	return hr;
}

void DemoScene::DescriptorSetInitialize()
{
}
