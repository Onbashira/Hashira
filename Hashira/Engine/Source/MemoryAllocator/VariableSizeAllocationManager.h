#pragma once
#include <map>

namespace Hashira {
	class VariableSizeAllocationManager
	{
	public:
		struct FreeBlockInfo;
		using OffsetType = size_t;
		using FreeBlocksByOffsetMap = std::map<size_t, FreeBlockInfo, std::less<OffsetType>>;
		//同じサイズのブロックが複数ある可能性があるため、2番目のマップはマルチマップ
		using FreeBlocksBySizeMap = std::multimap<size_t, FreeBlocksByOffsetMap::iterator, std::less<OffsetType>>;

		struct FreeBlockInfo
		{
			OffsetType Size;
			FreeBlocksBySizeMap::iterator OrderBySizeIt;

			FreeBlockInfo(OffsetType size) : Size(size) {};

		};

	private:

		FreeBlocksByOffsetMap _freeBlocksByOffset;

		FreeBlocksBySizeMap _freeBlocksBySize;

		OffsetType _freeSize;

		OffsetType _maxSize;

		OffsetType _currentAlignment;

	public:

		VariableSizeAllocationManager(OffsetType maxSize, OffsetType alignment);

		~VariableSizeAllocationManager();

		OffsetType Allocate(OffsetType size);

		void Free(OffsetType offset, OffsetType size);

	private:

		void AddNewBlock(OffsetType offset, OffsetType size);

		void ResetCurrAlignment();


	};

}