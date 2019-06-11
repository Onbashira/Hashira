#pragma once

namespace Hashira {

	//n�o�b�t�@�����O���T�|�[�g����N���X�i�_�u���o�b�t�@�����O�Ȃǁj
	template<typename T, unsigned int BufferNum>
	class MultipleBuffer
	{
	public:

	private:

		unsigned int _currentIndex;

		unsigned int _maxBufferSize;

		std::array<std::unique_ptr<T>, BufferNum> _buffer;

	public:

		MultipleBuffer() : _currentIndex(0), _maxBufferSize(BufferNum)
		{

		};

		~MultipleBuffer()
		{

		};

		//����Ώۂ����̃o�b�t�@�؂�ւ���i�t���b�s���O
		MultipleBuffer<T, BufferNum>& FlipBuffer()
		{
			_currentIndex = (_maxBufferSize + _currentIndex + 1) % _maxBufferSize;
			return *this;
		};

		//���݂̃o�b�t�@�ւ̃|�C���^�̎擾
		std::unique_ptr<T>& Get()
		{
			return this->_buffer[_currentIndex];
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
}