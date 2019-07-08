#include "stdafx.h"
#include "Descriptor.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "DescriptorHeap.h"

void Hashira::Descriptor::Discard()
{
	
}

void Hashira::Descriptor::Release()
{
	if (_parentHeap) {
		_parentHeap->ReleaseDescriptor(this);
	}
}
