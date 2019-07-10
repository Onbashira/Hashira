#include "DefaultRawMemoryAllocator.h"
#include "Engine/Source/Utility/BasicTypes.h"

Hashira::DefaultRawMemoryAllocator::DefaultRawMemoryAllocator()
{
}

void * Hashira::DefaultRawMemoryAllocator::Allocate(size_t Size, const char * dbgDescription, const char * dbgFileName, const int dbgLineNumber)
{
#ifdef _DEBUG
	return new Uint8[Size + 16] + 16;
#else
	return new Uint8[Size];
#endif
}

void Hashira::DefaultRawMemoryAllocator::Free(void * Ptr)
{
#ifdef _DEBUG
	delete[](reinterpret_cast<Uint8*>(Ptr) - 16);
#else
	delete[] reinterpret_cast<Uint8*>(Ptr);
#endif
}

Hashira::DefaultRawMemoryAllocator & Hashira::DefaultRawMemoryAllocator::GetAllocator()
{
	static DefaultRawMemoryAllocator Allocator;
	return Allocator;
}
