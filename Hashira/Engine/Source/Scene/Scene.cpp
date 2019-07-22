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
		//_mainCamera->GetDepthStencil().ClearDepthStencil(list.lock());
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

	_renderContext->WaitForGPU(_renderContext->GetCommandQueue().lock(), true);

	_renderContext->ClearCmdLists();

	_renderContext->ResetCurrentCommandAllocator();
	
}

void Hashira::Scene::Discard()
{
	//!デスクリプタヒープ
	_renderContext->Discard();;
	_mainCamera->Discard();
}

