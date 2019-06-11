#include "stdafx.h"
#include "RenderingPass.h"
#include "Engine/Source/CommandList/CommandList.h"


Hashira::RenderingPass::RenderingPass(std::shared_ptr<CommandList> cmdList, std::shared_ptr<GameHeap>& _gameHeap)
{
}

Hashira::RenderingPass::~RenderingPass()
{
}

std::shared_ptr<Hashira::CommandList> Hashira::RenderingPass::GetCommandList()
{
	return _cmdList;
}
