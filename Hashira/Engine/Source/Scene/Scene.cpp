#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Camera/Camera.h"
#include "Engine/Source/CoreSystem/Framework.h"


Hashira::CameraInitInfo GetDefaultCameraInfo()
{
	Hashira::CameraInitInfo ret{};
	Hashira::Uint32 x, y;
	Hashira::Framework::GetInstance().GetWindowSize(x, y);
	ret.width = static_cast<float>(x);
	ret.height = static_cast<float>(y);
	ret.fov = Hashira::DegToRad(90.0f);
	ret.nearClip = 0.01f;
	ret.farClip = 1000.0f;
	ret.position = Hashira::Vector3(0.0f, 0.0f, -10.0f);
	ret.target = Hashira::Vector3();
	ret.up = Hashira::Vector3::up;

	return ret;
}


Hashira::Scene::Scene(Uint32 viewDescMaxNum, Uint32 rtvDescriptorNum, Uint32 dsvDescriptorNum, Uint32 samplerDescriptorNum):
_renderingDevice(Framework::GetInstance().GetRenderingManagre().GetRenderingDevice()),
_renderContext(Framework::GetInstance().GetRenderingManagre().CreateRenderContext()),
_mainCamera()
{

	//RenderingContextCreate
	_renderContext->Initialize(viewDescMaxNum, rtvDescriptorNum, dsvDescriptorNum, samplerDescriptorNum);

	_mainCamera = std::make_shared<Camera>();

	//Initialize main camera with default settings;
	CameraInitInfo cInfo = GetDefaultCameraInfo();
	_mainCamera->Initialize(_renderContext, cInfo);

	//Initialize SceneConstant
	_sceneConstant.Initialize(_renderingDevice->GetD3D12Device(), 1, true);

	_sceneConstantDescriptor = _renderContext->GetViewDescriptorHeap()->Allocate();

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = _sceneConstant.GetResource()->GetGPUVirtualAddress();
	desc.SizeInBytes = Util::Alignment256Bytes(sizeof(SceneConstant));
	_renderingDevice->GetD3D12Device()->CreateConstantBufferView(&_sceneConstantDescriptor, &desc);
}

Hashira::Scene::~Scene()
{
	Discard();
}

void Hashira::Scene::SceneBegin()
{
	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::BEGIN).lock();
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list);

	//マスターレンダーターゲットのクリア
	{
		_renderContext->GetSwapChain()->ClearScreen(list);
	}

	//メインカメラデプスのクリア
	{
		//_mainCamera->ClearCurrentDepthStencil(list);
	}

	list->CloseCommandList();
	_renderContext->PushBackCmdList(list);
}

void Hashira::Scene::ExecutePath()
{

}

void Hashira::Scene::SceneEnd()
{


	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::END).lock();
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list);

	_renderContext->GetSwapChain()->SetStatePresent(list);
	list->CloseCommandList();
	_renderContext->PushBackCmdList(list);
	
	_renderContext->ExecuteCmdList3DQueue();

	_renderContext->Present(1,0);
	
	//レンダーターゲットフリッピング
	_renderContext->Flip();
	_mainCamera->FlipDepthStencilBuffer();
	_renderContext->WaitForGPU(_renderContext->GetCommandQueue().lock(), true);

	_renderContext->ClearCmdLists();

	_renderContext->ResetCurrentCommandAllocator();
	
}

void Hashira::Scene::Discard()
{
	//!デスクリプタヒープ
	_renderContext->Discard();
	_mainCamera->Discard();
	_sceneConstant.Discard();
	_sceneConstantDescriptor.Free();
}

