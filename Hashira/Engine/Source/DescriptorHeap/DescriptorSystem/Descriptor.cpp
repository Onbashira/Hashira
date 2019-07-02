#include "stdafx.h"
#include "Descriptor.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "DescriptorHeap.h"

void Hashira::DescriptorSystem::Descriptor::Discard()
{
	
}

void Hashira::DescriptorSystem::Descriptor::Release()
{
	if (_parentHeap) {
		_parentHeap->ReleaseDescriptor(this);
	}
}
