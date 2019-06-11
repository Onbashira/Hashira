#include "stdafx.h"
#include "SpriteRenderer.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"

Hashira::SpriteRenderer::SpriteRenderer()
{
}


Hashira::SpriteRenderer::~SpriteRenderer()
{
}

void Hashira::SpriteRenderer::Draw(std::shared_ptr<CommandList>& cmdList, GameObject * ojcet, GeometryState * geometryState, ModelMesh * mesh)
{

	auto descriptorsItr = mesh->GetDescriptors().begin();
	//Material View Set
	cmdList->SetGraphicsRootDescriptorTable(0, descriptorsItr->lock()->gpuAddress);
	//IA Stage
	
	auto& gState = mesh->GetGeometryState();
	cmdList->IASetIndexBuffer(gState->GetIndexBufferView());
	cmdList->IASetVertexBuffers(0,1,gState->GetVertexBufferView()->data());
	cmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->DrawIndexedInstanced(static_cast<unsigned int >(mesh->GetIndexBuffer()->GetElementCount()), 1, 0, 0,0);

}

void Hashira::SpriteRenderer::SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState)
{
	cmdList->SetPipelineState(pipelineState.lock());
}

void Hashira::SpriteRenderer::SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature)
{
	cmdList->SetGraphicsRootSignature(rootSignature.lock());

}
