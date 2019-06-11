#pragma once
namespace Hashira {

	class D3D12Device;

	class CommandAllocator
	{
	public:

	private:

		bool _isDuringExecuting;

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> _allocator;

		String _name;


	public:

		CommandAllocator();

		~CommandAllocator();

		/**
		* @fn
		* @brief �R�}���h���X�g�̍쐬
		* @param[in] listType ���X�g�̃^�C�v
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Initialize(D3D12_COMMAND_LIST_TYPE listType);

		/**
		* @fn
		* @brief �R�}���h���X�g�̍쐬
		* @param[in] device �쐬�Ɏg���f�o�C�X
		* @param[in] listType ���X�g�̃^�C�v
		* @return ���U���g�@S_OK�Ő���
		*/
		HRESULT Initialize(std::shared_ptr<D3D12Device>& device, D3D12_COMMAND_LIST_TYPE listType);

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator>& GetAllocator();

		void SetName(String name);
		
		String& GetName();

		void ResetAllocator();
		void ExecutedAllocator() {
			_isDuringExecuting = true;
		};

		void Discard();

	private:

	};

}