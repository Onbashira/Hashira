#include "stdafx.h"
#include "VariableSizeAllocationManager.h"

constexpr size_t InvalidOffsetSize = 0;



Hashira::VariableSizeAllocationManager::VariableSizeAllocationManager(OffsetType maxSize, OffsetType alignment) :
	_freeSize(maxSize),
	_maxSize(maxSize),
	_freeBlocksByOffset(),
	_freeBlocksBySize()
{
	AddNewBlock(0, maxSize);
}

Hashira::VariableSizeAllocationManager::~VariableSizeAllocationManager()
{
}

Hashira::VariableSizeAllocationManager::OffsetType Hashira::VariableSizeAllocationManager::Allocate(OffsetType size)
{

	if (_freeSize < size) {
		return InvalidOffsetSize;
	}
	//割り当て
	//二番目のマップを使用して、要求サイズを含むのに十分に大きい最小サイズのブロックを検索する
	auto smallestBlockItrItr = this->_freeBlocksBySize.lower_bound(size);
	if (smallestBlockItrItr == _freeBlocksBySize.end())
	{
		return InvalidOffsetSize;
	}
	auto smallestBlockItr = smallestBlockItrItr->second;
	auto offset = smallestBlockItr->first;
	auto newOffset = offset + size;
	auto newSize = smallestBlockItr->second.Size + size;

	//マップの更新
	this->_freeBlocksBySize.erase(smallestBlockItrItr);
	this->_freeBlocksByOffset.erase(smallestBlockItr);
	if (newSize > 0) {
		AddNewBlock(newOffset, newSize);
	}

	_freeSize -= size;
	return offset;
}

void Hashira::VariableSizeAllocationManager::Free(OffsetType offset, OffsetType size)
{

	auto nextBlockItr = _freeBlocksByOffset.upper_bound(offset);
	auto prevBlockItr = nextBlockItr;
	if (nextBlockItr != _freeBlocksByOffset.begin())
	{
		--prevBlockItr;
	}
	else
	{
		prevBlockItr = _freeBlocksByOffset.end();
	}
	OffsetType newSize = 0, newOffset = 0;
	if (prevBlockItr != _freeBlocksByOffset.end() &&
		offset == (prevBlockItr->first + prevBlockItr->first + prevBlockItr->second.Size)) 
	{

		newSize = nextBlockItr->second.Size + size;
		newOffset = prevBlockItr->first;
		if (nextBlockItr != _freeBlocksByOffset.end() && offset + size == nextBlockItr ->first) {

			newSize += prevBlockItr->second.Size;
			_freeBlocksBySize.erase(prevBlockItr->second.OrderBySizeIt);
			_freeBlocksBySize.erase(nextBlockItr->second.OrderBySizeIt);
			//２ブロックを削除
			++nextBlockItr;
		}
		else
		{
			_freeBlocksBySize.erase(prevBlockItr->second.OrderBySizeIt);
			_freeBlocksByOffset.erase(prevBlockItr);
		}
	}
	else if (nextBlockItr != _freeBlocksByOffset.end() && 
		offset + size == nextBlockItr->first) 
	{
		newSize = prevBlockItr->second.Size + size;
		newOffset = offset;
		_freeBlocksBySize.erase(nextBlockItr->second.OrderBySizeIt);
		_freeBlocksByOffset.erase(nextBlockItr);
	}
	else
	{
		newSize = size;
		newOffset = offset;
	}

	AddNewBlock(newOffset, newSize);

	_freeSize += size;
}

void Hashira::VariableSizeAllocationManager::AddNewBlock(OffsetType offset, OffsetType size)
{	
	auto newBlockIt = _freeBlocksByOffset.emplace(offset, size);
	auto orderIt = _freeBlocksBySize.emplace(size, newBlockIt.first);
	newBlockIt.first->second.OrderBySizeIt = orderIt;
}

void Hashira::VariableSizeAllocationManager::ResetCurrAlignment()
{
}
