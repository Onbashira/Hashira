#include "stdafx.h"
#include "InternalCommandAllocator.h"


Hashira::InternalCommandAllocator::InternalCommandAllocator()
{
}


Hashira::InternalCommandAllocator::~InternalCommandAllocator()
{
}

void Hashira::InternalCommandAllocator::Reset()
{
	_allocatedMemory.clear();
}
