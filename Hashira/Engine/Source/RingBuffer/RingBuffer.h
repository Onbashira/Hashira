#pragma once
#include <deque>
#include "Engine/Source/MemoryAllocator/STDAllocator.h"
namespace Hashira {
	class RingBuffer
	{
	public:
		struct FrameHeadAttribute {
			unsigned long fenceValue;
			unsigned int offset;
			unsigned int size;
			FrameHeadAttribute(unsigned long fenceValue, unsigned int offset, unsigned int type)noexcept :
				fenceValue(fenceValue), offset(offset), size(size)
			{}

		};

		static constexpr unsigned int InvalidOffset = -1;

	protected:

		std::deque<FrameHeadAttribute, STDAllocatorRawMem<FrameHeadAttribute>> _completedFrameHeads;

		unsigned int _head = 0;

		unsigned int _tail = 0;

		unsigned int _maxSize = 0;

		unsigned int _usedSize = 0;

		unsigned int _currentSize = 0;

	public:

		RingBuffer(unsigned int maxSize, IMemoryAllocator& allocator) noexcept;

		RingBuffer(RingBuffer& buffer) = delete;

		RingBuffer(RingBuffer&& buffer) noexcept;

		RingBuffer& operator = (RingBuffer&& buffer) noexcept;

		RingBuffer& operator = (RingBuffer& buffer) = delete;

		~RingBuffer();

		//void Initialize(unsigned int maxSize);

		void Discard();

		unsigned int Allocate(unsigned int size, unsigned int alignment);

		void CompleteCurrentFrame(unsigned long fenceValue);

		void ReleaseCompletedFrame(unsigned long completedFenceValue);

		unsigned int GetMaxSize() { return _maxSize; };

		unsigned int GetUsedSize() { return _usedSize; };

		bool IsFull()const { return _usedSize == _maxSize; };

		bool IsEmpty()const { return _usedSize == 0; };


	};
}