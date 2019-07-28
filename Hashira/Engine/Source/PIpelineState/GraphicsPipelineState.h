#pragma once


namespace Hashira
{

	class D3D12Device;
	class Shader;
	class RootSignature;
	struct BlendDesc;
	struct RasterizerDesc;
	struct DepthStencilDesc;
	struct InputLayoutDesc;

	struct RenderTargetBlendDesc {
		bool			isBlendEnable;
		bool			isLogicBlendEnable;
		D3D12_BLEND		srcBlendColor;
		D3D12_BLEND		dstBlendColor;
		D3D12_BLEND_OP	blendOpColor;
		D3D12_BLEND		srcBlendAlpha;
		D3D12_BLEND		dstBlendAlpha;
		D3D12_BLEND_OP	blendOpAlpha;
		D3D12_LOGIC_OP	logicOp;
		UINT8			writeMask;

		RenderTargetBlendDesc();
		~RenderTargetBlendDesc();
	};

	struct BlendDesc
	{
		bool					isAlphaToCoverageEnable;
		bool					isIndependentBlend;
		Uint32					sampleMask;
		RenderTargetBlendDesc	rtDesc[8];
		BlendDesc();
		~BlendDesc();

	};

	struct RasterizerDesc
	{
		D3D12_FILL_MODE		fillMode;
		D3D12_CULL_MODE		cullMode;
		bool				isFrontCCW;
		Int32				depthBias;
		float				depthBiasClamp;
		float				slopeScaledDepthBias;
		bool				isDepthClipEnable;
		bool				isMultisampleEnable;
		bool				isAntialiasedLineEnable;
		bool				isConservativeRasterEnable;
		RasterizerDesc();
		~RasterizerDesc();

	};

	struct DepthStencilDesc
	{
		bool						isDepthEnable;
		bool						isDepthWriteEnable;
		D3D12_COMPARISON_FUNC		depthFunc;
		bool						isStencilEnable;
		Uint8						stencilReadMask;
		Uint8						stencilWriteMask;
		D3D12_DEPTH_STENCILOP_DESC	stencilFrontFace;
		D3D12_DEPTH_STENCILOP_DESC	stencilBackFace;

		DepthStencilDesc();
		~DepthStencilDesc();
	};

	struct InputLayoutDesc
	{
		const D3D12_INPUT_ELEMENT_DESC* pElements;
		Uint32							numElements;

		InputLayoutDesc();
		~InputLayoutDesc();

	};

	struct GraphicsPipelineStateDesc
	{
		RootSignature* rootSignature;
		Shader* vs;
		Shader* ps;
		Shader* gs;
		Shader* ds;
		Shader* hs;
		Shader* cs;
		BlendDesc blendDesc;
		RasterizerDesc rasterizerDesc;
		DepthStencilDesc depthStencilDesc;
		InputLayoutDesc inputLayout;
		D3D12_PRIMITIVE_TOPOLOGY primitiveTopology;
		Uint32 numRTVs;
		DXGI_FORMAT rtvFormats[8];
		DXGI_FORMAT dsvFormat;
		Uint32 multiSamplerCount;

		GraphicsPipelineStateDesc();
		~GraphicsPipelineStateDesc();

	};

	class GraphicsPipelineState
	{
	public:
	private:
		//!パイプラインステート
		Microsoft::WRL::ComPtr<ID3D12PipelineState> _pso;

		GraphicsPipelineStateDesc	_desc;
	public:

		GraphicsPipelineState();
		~GraphicsPipelineState();
		
		HRESULT Initialize(std::shared_ptr<D3D12Device>& device,const GraphicsPipelineStateDesc& desc);

		Microsoft::WRL::ComPtr<ID3D12PipelineState>& GetPSO();

		void Discard();


	private:


	};

}