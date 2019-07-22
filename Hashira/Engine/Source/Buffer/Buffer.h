#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include <tuple>
#include <string>


namespace Hashira {

	class CommandList;
	class D3D12Device;

	struct BufferUsage {
		enum Type {
			Constant,
			ShaderResource,
			UnorderedAccess,
			Rnedertarget,
			DepthStencill,
			Max,
		};
	};

	struct BufferMemoryPoolType
	{
		enum Type
		{
			L0,
			L1,
		};
	};

	class Buffer
	{

	private:

	protected:

		//!���݂̃��\�[�X�X�e�[�g
		D3D12_RESOURCE_STATES _currentResourceState;

		//!�}�b�v���ꂽ�|�C���^
		UCHAR* _pDst;

		//!�N���A�o�����[
		D3D12_CLEAR_VALUE _clearValue;

		//!UAV
		bool _isAllowUAV;

		bool _isDynamic;
		
		D3D12_RESOURCE_DESC _resDesc;

		D3D12_HEAP_PROPERTIES _heapProp;

		//!���\�[�X�{��
		Microsoft::WRL::ComPtr<ID3D12Resource> _resource;

		//!���O
		std::string _name;

	private:

	public:


		Buffer(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		Buffer(const Buffer& other);

		Buffer(Buffer&& value);

		Buffer& operator= (const Buffer& value);

		Buffer& operator= (Buffer&& value);

		virtual ~Buffer();

		/**
		* @fn
		* @brief ���������̃o�b�t�@�̃��j�[�N�ȃ|�C���^�̎擾
		* @return ���j�[�N�|�C���^
		*/
		static  std::unique_ptr<Hashira::Buffer> CreateUnique();

		/**
		* @fn
		* @brief ���������̃o�b�t�@�̋��Q�Ƃ̎擾
		* @return ���Q��
		*/
		static  std::shared_ptr<Hashira::Buffer> CreateShared();

		/**
		* @fn
		* @brief �o�b�t�@�̍쐬
		* @param[in] heapProps �v���p�e�B
		* @param[in] flags �q�[�v�t���O
		* @param[in] resourceDesc �o�b�t�@�̃f�X�N���v�V����
		* @param[in] state �����X�e�[�g
		* @param[in] clearValue �N���A�o�����[
		* @return ���U���g
		*/
		virtual HRESULT Initialize(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		/**
		* @fn
		* @brief �o�b�t�@�̍쐬
		* @param[in] device �f�o�C�X
		* @param[in] heapProps �v���p�e�B
		* @param[in] flags �q�[�v�t���O
		* @param[in] resourceDesc �o�b�t�@�̃f�X�N���v�V����
		* @param[in] state �����X�e�[�g
		* @param[in] clearValue �N���A�o�����[
		* @return ���U���g
		*/
		virtual HRESULT Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		/**
		* @fn
		* @brief �}�b�s���O
		* @param[in] subResource �T�u���\�[�X
		* @param[in] readRange �ǂݍ��݃����W
		* @return ���U���g
		*/
		virtual HRESULT Map(UINT subResource, D3D12_RANGE* readRange);

		/**
		* @fn
		* @brief �}�b�s���O����
		* @param[in] subResource �T�u���\�[�X
		* @param[in] writtenRange �������݃����W
		*/
		virtual void Unmap(UINT subResource, D3D12_RANGE* writtenRange);

		/**
		* @fn
		* @brief �}�b�s���O���ꂽ�|�C���^�̎擾
		* @return �o�b�t�@�Ƀ}�b�s���O���ꂽ�|�C���^
		*/
		UCHAR* GetMappedPointer();

		/**
		* @fn
		* @brief �o�b�t�@�j��
		*/
		void Discard();

		/**
		* @fn
		* @brief �o�b�t�@�X�V�E��������
		* @param[in] pSrc �\�[�X�ւ̃|�C���^
		* @param[in] size �T�C�Y
		* @param[in] dstOffset �I�t�Z�b�g
		*/
		virtual void Update(const void* pSrc, ULONG64 size, const size_t dstOffset);

		/**
		* @fn
		* @brief �o�b�t�@�X�V�E��������
		* @param[in] pSrc �������ݐ�ւ̃|�C���^
		* @param[in] size �T�C�Y
		* @param[in] dstOffset �I�t�Z�b�g
		*/
		virtual void Read(void* pDstBuffer, ULONG64 readSize, const unsigned int dstOffsest = 0);

		/**
		* @fn
		* @brief �o�b�t�@�X�V�E��������
		* @param[in] pSrc �\�[�X�ւ̃|�C���^
		* @param[in] size �T�C�Y
		* @param[in] dstOffset �I�t�Z�b�g
		* @param[in] readRange �ǂݍ��݃����W
		* @param[in] writtenRange �������݃����W
		*/
		void Update(const void* pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE* readRange, D3D12_RANGE* writtenRange);

		/**
		* @fn
		* @brief 256�o�C�g�A���C�����g���ăo�b�t�@�X�V�E��������
		* @param[in] pSrc �������ݐ�ւ̃|�C���^
		* @param[in] elementCount �P�v�f�T�C�Y
		* @param[in] elementCount �v�f��
		*/
		void Alignment256ByteUpdate(const void* pSrc, unsigned int elementSize, unsigned int elementCount = 1);

		/**
		* @fn
		* @brief �o�b�t�@�̃t�F�b�`
		* @return �o�b�t�@�ւ̎Q��
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource>& GetResource();

		/**
		* @fn
		* @brief �q�[�v�̏��̃t�F�b�`
		* @return �i�v���p�e�B�A�t���O�j
		*/
		const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> GetHeapPropaties();

		/**
		* @fn
		* @brief �o�b�t�@�̃f�X�N���v�V�����̃t�F�b�`
		* @return �f�X�N���v�V����
		*/
		const D3D12_RESOURCE_DESC GetResourceDesc();

		/**
		* @fn
		* @brief �o�b�t�@�X�e�[�g�̃t�F�b�`
		* @return �X�e�[�g
		*/
		const D3D12_RESOURCE_STATES& GetResourceState();

		/**
		* @fn
		* @brief �o�b�t�@�X�e�[�g�̃Z�b�g
		* @param[in] state �o�b�t�@�X�e�[�g
		*/
		void SetResourceState(D3D12_RESOURCE_STATES state);

		/**
		* @fn
		* @brief �o�b�t�@�X�e�[�g�̑J��
		* @param[in] list �R�}���h���X�g�̎Q��
		* @param[in] nextState �o�b�t�@�X�e�[�g
		* @return ���U���g
		*/
		HRESULT	ResourceTransition(CommandList* list, D3D12_RESOURCE_STATES nextState);

		/**
		* @fn
		* @brief �o�b�t�@�X�e�[�g�̑J��
		* @param[in] list �R�}���h���X�g�ւ̎�Q��
		* @param[in] nextState �o�b�t�@�X�e�[�g
		* @return ���U���g
		*/
		HRESULT	ResourceTransition(std::weak_ptr<CommandList> list, D3D12_RESOURCE_STATES nextState);

		/**
		* @fn
		* @brief �N���A�o�����[�̃t�F�b�`
		* @return �N���A�o�����[
		*/
		D3D12_CLEAR_VALUE GetClearValue();

		/**
		* @fn
		* @brief ���\�[�X�̖��O�̐ݒ�
		* @param[in] name ���O
		*/
		void SetName(std::string name);

	protected:

		Buffer();

	private:

	};
}