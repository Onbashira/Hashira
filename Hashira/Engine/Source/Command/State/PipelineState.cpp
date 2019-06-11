#include "PipelineState.h"

Hashira::PipelineState::PipelineState()
{
}

Hashira::PipelineState::~PipelineState()
{
}


std::weak_ptr<Hashira::PipelineStateObject> Hashira::PipelineState::GetPSO()
{
	return this->_pso;
}

std::weak_ptr<Hashira::RootSignature> Hashira::PipelineState::GetRS()
{
	return this->_rs;
}

void Hashira::PipelineState::SetPSO(std::weak_ptr<PipelineStateObject> pso)
{
 this->_pso = pso.lock();

}

void Hashira::PipelineState::SetRS(std::weak_ptr<RootSignature> rs)
{
	this->_rs = rs.lock();
}
