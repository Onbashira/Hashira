#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Camera/Camera.h"
#include "Engine/Source/CoreSystem/Framework.h"

Hashira::Scene::Scene(std::shared_ptr<RenderingDevice>& renderingDevice) :
	_renderingDevice(renderingDevice),
	_renderContext(Framework::GetInstance().GetRenderingManagre().CreateRenderContext()),
	_mainCamera()

{
	//RenderingContextCreate
	_renderContext->IntializeAllocators(renderingDevice->GetD3D12Device()->GetDevice());

	_mainCamera = std::make_shared<Camera>();

	CameraInitInfo cInfo{};
	Uint32 x, y;
	Framework::GetInstance().GetWindowSize(x,y);
	cInfo.width = static_cast<float >(x);
	cInfo.height = static_cast<float>(y);
	cInfo.fov = DegToRad(90.0f);
	_mainCamera->Initialize(_renderContext, cInfo);

	_sceneConstant.Initialize(renderingDevice->GetD3D12Device(), 1, true);

	_sceneConstantDescriptor = _renderContext->GetViewDescriptorHeap()->Allocate();

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc{};
	desc.BufferLocation = _sceneConstant.GetResource()->GetGPUVirtualAddress();
	desc.SizeInBytes = Util::Alignment256Bytes(sizeof(SceneConstant));
	renderingDevice->GetD3D12Device()->CreateConstantBufferView(&_sceneConstantDescriptor,&desc);

}

Hashira::Scene::~Scene()
{
	Discard();
}

void Hashira::Scene::SceneBegin()
{
	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::BEGIN);
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list.lock());

	//マスターレンダーターゲットのクリア
	{
		_renderContext->GetSwapChain()->ClearScreen(list.lock());
	}

	//メインカメラデプスのクリア
	{
		_mainCamera->ClearCurrentDepthStencil(list.lock());
	}

	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());
}

void Hashira::Scene::ExecutePath()
{
}

void Hashira::Scene::SceneEnd()
{


	//リソースアップデート用のコマンドリストのフェッチ
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::END);
	//コマンドリストを現在のアロケータでリセット
	_renderContext->ResetCommandList(list.lock());
	_renderContext->GetSwapChain()->SetStatePresent(list.lock());
	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());

	_renderContext->ExecuteCmdList3DQueue();

	_renderContext->Present(1, 0);
	
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
}

