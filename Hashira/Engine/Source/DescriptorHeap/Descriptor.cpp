#include "stdafx.h"
#include "Descriptor.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "DescriptorHeap.h"

void BindCPUDescriptor(unsigned int rootParam,Hashira::Descriptor::BindType bindType ,Hashira::Descriptor& descriptor, std::shared_ptr<Hashira::CommandList> list);

void Hashira::Descriptor::BindShader(unsigned int rootParamater, BindType bindType, std::shared_ptr<Hashira::CommandList> list)
{
	switch (viewType)
	{

	case Hashira::Descriptor::ViewType::CBV:
	case Hashira::Descriptor::ViewType::SRV:
	case Hashira::Descriptor::ViewType::UAV:
	case Hashira::Descriptor::ViewType::SAMP:

		BindCPUDescriptor(rootParamater, bindType, *this, list);
		break;
	case Hashira::Descriptor::ViewType::RTV:
	case Hashira::Descriptor::ViewType::DSV:
		
		break;

	case Hashira::Descriptor::ViewType::EMPTY:
		//none
		break;
	default:
		break;
	}
}

void BindCPUDescriptor(unsigned int rootParam, Hashira::Descriptor::BindType bindType, Hashira::Descriptor & descriptor, std::shared_ptr<Hashira::CommandList> list)
{
	switch (bindType)
	{
	case Hashira::Descriptor::BindType::Graphics:
		list->SetGraphicsRootDescriptorTable(rootParam, descriptor.gpuAddress);
		break;
	case Hashira::Descriptor::BindType::Compute:
		list->SetComputeRootDescriptorTable(rootParam, descriptor.gpuAddress);
		break;
	default:
		break;
	}
}