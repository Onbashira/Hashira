#include "RingBuffer.h"
#include "Engine/Source/Utility/Utility.h"

Hashira::RingBuffer::RingBuffer(unsigned int maxSize, IMemoryAllocator& allocator) noexcept :
	_completedFrameHeads(STD_ALLOCATOR_RAW_MEM(FrameHeadAttribute, allocator, "Allocator for deque<FrameHeadAttribs>")),
	_maxSize(maxSize)
{

}

Hashira::RingBuffer::RingBuffer(RingBuffer && buffer) noexcept :
	_completedFrameHeads(std::move(buffer._completedFrameHeads)), _tail(buffer._tail),
	_head(buffer._head), _maxSize(buffer._maxSize), _usedSize(buffer._usedSize), _currentSize(buffer._currentSize)
{
	this->_currentSize = 0;
	this->_head = 0;
	this->_maxSize = 0;
	this->_tail = 0;
	this->_usedSize = 0;
}

Hashira::RingBuffer & Hashira::RingBuffer::operator=(RingBuffer && buffer) noexcept
{
	_completedFrameHeads = std::move(buffer._completedFrameHeads);
	_tail = buffer._tail;
	_head = buffer._head;
	_maxSize = buffer._maxSize;
	_usedSize = buffer._usedSize;
	_currentSize = buffer._currentSize;

	buffer._maxSize = 0;
	buffer._tail = 0;
	buffer._head = 0;
	buffer._usedSize = 0;
	buffer._currentSize = 0;

	return *this;
}

Hashira::RingBuffer::~RingBuffer()
{
	Discard();
}

void Hashira::RingBuffer::Discard()
{
	this->_completedFrameHeads.clear();
	this->_currentSize = 0;
	this->_head = 0;
	this->_maxSize = 0;
	this->_tail = 0;
	this->_usedSize = 0;
}

unsigned int Hashira::RingBuffer::Allocate(unsigned int size, unsigned int alignment)
{
	if (size <= 0) {
		return InvalidOffset;
	}
	if (IsFull()) {
		return InvalidOffset;
	}
	if (!Util::IsPowerOfTwo(alignment)) {
		return InvalidOffset;
	}
	size = Util::Align(size, alignment);



	if (_usedSize + size > _maxSize) {
		return InvalidOffset;
	}

	auto alignmentHead = size = Util::Align(_head, alignment);

	if (_head >= _tail)
	{

		//                                         AlignedHead
		//                     Tail          Head  |            MaxSize
		//                     |                |  |            |
		//  [                  xxxxxxxxxxxxxxxxx...             ]
		//                                         
		//
		if (alignmentHead + size <= _maxSize)
		{
			unsigned int offset = alignmentHead;
			unsigned int adjustSize = size + (alignmentHead - _head);
			_head += adjustSize;
			_usedSize += adjustSize;
			_currentSize += adjustSize;
			return offset;
		}
		else if (size <= _tail)
		{
			// Allocate from the beginning of the buffer
			//
			//                                                    
			// Offset              Tail          Head               MaxSize
			//  |                  |                |<---AddSize--->|
			//  [                  xxxxxxxxxxxxxxxxx++++++++++++++++]
			// 
			unsigned int addSize = (_maxSize - _head) + size;
			_head = size;
			_usedSize += addSize;
			_currentSize += addSize;
			return 0;
		}
	}
	else if (alignmentHead + size <= _tail) {
		//          AlignedHead
		//    Head  |              Tail             
		//       |  |              |             
		//  [xxxx...               xxxxxxxxxxxxxxxxxxxxxxxxxx]
		//
		auto offset = alignmentHead;
		auto adjustedSize = size + (alignmentHead - _head);
		_head += adjustedSize;
		_usedSize += adjustedSize;
		_currentSize += adjustedSize;
		return offset;
	}
	return InvalidOffset;
}

void Hashira::RingBuffer::CompleteCurrentFrame(unsigned long fenceValue)
{
	if (_currentSize != 0) {
		_completedFrameHeads.emplace_back(fenceValue, _head, _currentSize);
		_currentSize = 0;
	}
}

void Hashira::RingBuffer::ReleaseCompletedFrame(unsigned long completedFenceValue)
{

	while (!_completedFrameHeads.empty() && _completedFrameHeads.front().fenceValue <= completedFenceValue)
	{
		const auto &OldestFrameTail = _completedFrameHeads.front();
		//if (OldestFrameTail.size <= _usedSize)
		//{
		//	_sleep(1);
		//}
		_usedSize -= OldestFrameTail.size;
		_tail = OldestFrameTail.offset;
		_completedFrameHeads.pop_front();
	}
	if (IsEmpty()) {
		_completedFrameHeads.clear();
		_tail = _head;
	}
}
