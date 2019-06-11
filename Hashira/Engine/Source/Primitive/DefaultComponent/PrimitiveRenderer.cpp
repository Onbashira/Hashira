#include "stdafx.h"
#include "PrimitiveRenderer.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Command/State/GeometryState.h"
#include "Engine/Source/Mesh/ModelMesh.h"
#include "Engine/Source/Mesh/MeshBuffer.h"
#include "Engine/Source/Mesh/MeshHeap.h"

Hashira::PrimitiveRenderer::PrimitiveRenderer()
{
}


Hashira::PrimitiveRenderer::~PrimitiveRenderer()
{
}

void Hashira::PrimitiveRenderer::Draw(std::shared_ptr<CommandList>& cmdList, GameObject * ojcet, GeometryState * geometryState, ModelMesh * mesh)
{

	auto& descriptorsItr = mesh->GetDescriptors().begin();
	//Camera View Set
	cmdList->SetGraphicsRootDescriptorTable(0, descriptorsItr->lock()->gpuAddress);
	//transform View Set
	++descriptorsItr;
	cmdList->SetGraphicsRootDescriptorTable(1, descriptorsItr->lock()->gpuAddress);

	//IA Stage
	auto& gState = mesh->GetGeometryState();
	cmdList->IASetIndexBuffer(gState->GetIndexBufferView());
	cmdList->IASetVertexBuffers(0,1,gState->GetVertexBufferView()->data());
	cmdList->IASetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	cmdList->DrawIndexedInstanced(static_cast<unsigned int >(mesh->GetIndexBuffer()->GetElementCount()), 1, 0, 0,0);

}

void Hashira::PrimitiveRenderer::SetPipelineState(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::PipelineStateObject> pipelineState)
{
	cmdList->SetPipelineState(pipelineState.lock());
}

void Hashira::PrimitiveRenderer::SetRootSignature(std::shared_ptr<CommandList>& cmdList, std::weak_ptr<Hashira::RootSignature> rootSignature)
{
	cmdList->SetGraphicsRootSignature(rootSignature.lock());

}
