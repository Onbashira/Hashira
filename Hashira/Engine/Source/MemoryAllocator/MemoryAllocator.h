#pragma once

namespace K3D
{
	/// Base interface for a raw memory allocator
	class IMemoryAllocator
	{
	public:
		/// Allocates block of memory
		virtual void* Allocate(size_t Size, const char* dbgDescription, const char* dbgFileName, const int dbgLineNumber) = 0;

		/// Releases memory
		virtual void Free(void *Ptr) = 0;
	};

}