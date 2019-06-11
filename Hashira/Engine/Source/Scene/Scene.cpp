#include "stdafx.h"
#include "Scene.h"
#include "Engine/Source/DescriptorHeap/GameHeap.h"
#include "Engine/Source/Device/RenderingDevice.h"
#include "Engine/Source/Rendering/RenderContext/RenderContext.h"
#include "Engine/Source/Device/D3D12Device.h"
#include "Engine/Source/Camera/Camera.h"
#include "Engine/Source/Rendering/RenderingPass/RenderingPass.h"


Hashira::Scene::Scene(std::shared_ptr<RenderingDevice>& renderingDevice) :
	_renderingDevice(renderingDevice),
	_renderContext(Framework::GetInstance().GetRenderingManagre().CreateRenderContext()),
	_gameHeap(new GameHeap(renderingDevice->GetD3D12Device(), Hashira::GameHeap::GameHeapDesc())),
	_mainCamera(new Camera(_gameHeap))

{
	//RenderingContextCreate

	unsigned int w = 0,h = 0;
	Hashira::Framework::GetInstance().GetWindowSize(w,h);
	//�J�����̃C�j�V�����C�Y
	_mainCamera->InitializeCamera(Hashira::CAMERA_MODE::Perspective,static_cast<float>(w),static_cast<float>( h),
		1.0f, 500.0f, Vector3(0.0f, 15.0f, -10.0f), Vector3(0.0f,0.0f,0.0f), Vector3::up);
	//�J�����̃f�v�X�X�e���V���o�b�t�@�̃C�j�V�����C�Y
	_mainCamera->InitializeCameraDepthStencill(DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT, w, h);
	//�J����CPU�r���[�̍쐬
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc = {};
	desc.BufferLocation = _mainCamera->GetCameraBuffer().GetResource()->GetGPUVirtualAddress();
	desc.SizeInBytes = Util::ConstantBufferAlign(sizeof(CameraInfo));//512Byte
	_gameHeap->CreateCBView(desc);
	
}

Hashira::Scene::~Scene()
{
	Discard();
}

void Hashira::Scene::SceneBegin()
{
	//���\�[�X�A�b�v�f�[�g�p�̃R�}���h���X�g�̃t�F�b�`
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::BEGIN);
	//�R�}���h���X�g�����݂̃A���P�[�^�Ń��Z�b�g
	_renderContext->ResetCommandList(list.lock());

	//�}�X�^�[�����_�[�^�[�Q�b�g�̃N���A
	{
		_renderContext->GetSwapChain()->ClearScreen(list.lock());
	}

	//���C���J�����f�v�X�̃N���A
	{
		_mainCamera->GetDepthStencil().ClearDepthStencil(list.lock());
	}

	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());
}

void Hashira::Scene::ExecutePath()
{
}

void Hashira::Scene::SceneEnd()
{


	//���\�[�X�A�b�v�f�[�g�p�̃R�}���h���X�g�̃t�F�b�`
	auto& list = _renderContext->GetResourceUpdateCmdList(RenderContext::RC_COMMAND_LIST_TYPE::END);
	//�R�}���h���X�g�����݂̃A���P�[�^�Ń��Z�b�g
	_renderContext->ResetCommandList(list.lock());
	_renderContext->GetSwapChain()->SetStatePresent(list.lock());
	list.lock()->CloseCommandList();
	_renderContext->PushBackCmdList(list.lock());

	_renderContext->ExecuteCmdList3DQueue();

	_renderContext->Present(1, 0);
	
	//�����_�[�^�[�Q�b�g�t���b�s���O
	_renderContext->Flip();

	_renderContext->WaitForGPU(_renderContext->GetCommandQueue().lock(), true);

	_renderContext->ClearCmdLists();

	_renderContext->ResetCurrentCommandAllocator();
	
}

void Hashira::Scene::Discard()
{
	//!�f�X�N���v�^�q�[�v
	_gameHeap->Discard();
	_renderContext->Discard();;
	_mainCamera->Discard();
}

