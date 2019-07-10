#pragma once
#include "MemoryAllocator.h"

namespace Hashira
{

	class DefaultRawMemoryAllocator : public IMemoryAllocator
	{
	public:
		DefaultRawMemoryAllocator();

		/// Allocates block of memory
		virtual void* Allocate(size_t Size, const char* dbgDescription, const char* dbgFileName, const  int dbgLineNumber)override;

		/// Releases memory
		virtual void Free(void *Ptr)override;

		static DefaultRawMemoryAllocator& GetAllocator();

	private:
		DefaultRawMemoryAllocator(const DefaultRawMemoryAllocator&) = delete;
		DefaultRawMemoryAllocator(DefaultRawMemoryAllocator&&) = delete;
		DefaultRawMemoryAllocator& operator = (const DefaultRawMemoryAllocator&) = delete;
		DefaultRawMemoryAllocator& operator = (DefaultRawMemoryAllocator&&) = delete;
	};

}