#pragma once

namespace Hashira {

	//nバッファリングをサポートするクラス（ダブルバッファリングなど）
	template<typename T, unsigned int BufferNum>
	class MultipleUniquePtrBuffer
	{
	public:

	private:

		unsigned int _currentIndex;

		unsigned int _maxBufferSize;

		std::array<std::unique_ptr<T>, BufferNum> _buffer;

	public:

		MultipleUniquePtrBuffer() : _currentIndex(0), _maxBufferSize(BufferNum)
		{

		};

		~MultipleUniquePtrBuffer()
		{

		};

		//操作対象を次のバッファ切り替える（フリッピング
		MultipleUniquePtrBuffer<T, BufferNum>& FlipBuffer()
		{
			_currentIndex = (_maxBufferSize + _currentIndex + 1) % _maxBufferSize;
			return *this;
		};

		//現在のバッファへのポインタの取得
		std::unique_ptr<T>& Get()
		{
			return this->_buffer[_currentIndex];
		};

		//指定インデックスのバッファへの参照の取得
		std::unique_ptr<T>& Get(Uint32 index)
		{
			return this->_buffer[index];
		};

		void SetAddressOf(unsigned int index, std::unique_ptr<T>&& buffer)
		{
			this->_buffer[index].reset();
			
			_buffer[index] = std::move(buffer);
		};

		void DiscardBuffers()
		{
			for (auto& buffer : _buffer) {
				buffer.reset();
			}
		};
	private:
	};

	template<typename T, unsigned int BufferNum>
	class MultipleBuffer
	{
	public:

	private:

		unsigned int _currentIndex;

		unsigned int _maxBufferSize;

		std::array<T, BufferNum> _buffer;

	public:

		MultipleBuffer() : _currentIndex(0), _maxBufferSize(BufferNum)
		{

		};

		~MultipleBuffer()
		{

		};

		//操作対象を次のバッファ切り替える（フリッピング
		MultipleBuffer<T, BufferNum>& FlipBuffer()
		{
			_currentIndex = (_maxBufferSize + _currentIndex + 1) % _maxBufferSize;
			return *this;
		};

		//現在のバッファへの参照取得
		T& Get()
		{
			return this->_buffer[_currentIndex];
		};

		//指定インデックスのバッファへの参照の取得
		T& Get(Uint32 index)
		{
			return this->_buffer[index];
		};

		void Set(unsigned int index, T&& buffer)
		{
			_buffer[index] = std::move(buffer);
		};

		void DiscardBuffers()
		{
		};
	private:
	};
}