#pragma once
namespace Hashira {

	class CommandList;


	class Descriptor {

		friend class DescriptorHeap;

	public:

	private:

		//ヒープのインデックス
		unsigned int _index;
		//CPUデスクリプタハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE _cpuHandle;
		//GPUデスクリプタハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE _gpuHandle;
		//親ヒープ
		DescriptorHeap* _parentHeap;
		//前
		Descriptor* _prev;
		//次
		Descriptor* _next;

	public:

		Descriptor() :
			_index(0), _cpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE()), _gpuHandle(D3D12_GPU_DESCRIPTOR_HANDLE()) {};

		Descriptor(unsigned int offset, const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpuAddress) :
			_index(offset), _cpuHandle(cpuHandle), _gpuHandle(gpuAddress) {};

		Descriptor(Descriptor&& other) {
			*this = std::move(other);
		};

		Descriptor(const Descriptor& other) {
			*this = other;
		};

		~Descriptor() {
			Release();
		};

		Descriptor& operator= (const Descriptor& other) {
			_index = other._index;
			_gpuHandle = other._gpuHandle;
			_cpuHandle = other._cpuHandle;
			return *this;
		};

		Descriptor& operator= (Descriptor&& other) {
			*this = other;

			other._index = 0;
			other._gpuHandle = D3D12_GPU_DESCRIPTOR_HANDLE();
			other._cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE();

			return *this;
		};

		D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle()
		{
			return _cpuHandle;
		};

		D3D12_GPU_DESCRIPTOR_HANDLE GetGpuHandle()
		{
			return _gpuHandle;
		};

		void Discard();

		void Release();

	private:


	};
}