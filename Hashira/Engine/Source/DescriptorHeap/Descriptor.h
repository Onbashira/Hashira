#pragma once
namespace Hashira {

	class CommandList;

	struct Descriptor {

		enum class ViewType {
			 EMPTY, CBV, SRV, UAV, RTV, DSV, SAMP
		};

		enum class BindType {
			Graphics,Compute
		};

		//ヒープ先頭からのオフセット
		unsigned int offset;
		//このビューのタイプ
		Descriptor::ViewType viewType;
		//CPUデスクリプタハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle;
		//GPUデスクリプタハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE gpuAddress;

		Descriptor() :
			offset(0), viewType(Descriptor::ViewType::EMPTY), cpuHandle(D3D12_CPU_DESCRIPTOR_HANDLE()), gpuAddress(D3D12_GPU_DESCRIPTOR_HANDLE()) {};

		Descriptor(unsigned int offset, Descriptor::ViewType viewType,const D3D12_CPU_DESCRIPTOR_HANDLE& cpuHandle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpuAddress) :
			offset(offset), viewType(viewType), cpuHandle(cpuHandle), gpuAddress(gpuAddress) {};

		Descriptor(Descriptor&& other) {
			*this = std::move(other);

		};

		Descriptor& operator= (Descriptor&& other) {
			*this = other;

			other.offset = 0;
			other.viewType = Descriptor::ViewType::EMPTY;
			other.gpuAddress = D3D12_GPU_DESCRIPTOR_HANDLE();
			other.cpuHandle = D3D12_CPU_DESCRIPTOR_HANDLE();

			return *this;
		};

		void BindShader(unsigned int rootParamater,BindType bindType,std::shared_ptr<Hashira::CommandList> list);

	private:

		Descriptor(const Descriptor& other) {
			*this = other;
		};

		Descriptor& operator= (const Descriptor& other) {
			offset = other.offset;
			viewType = other.viewType;
			gpuAddress = other.gpuAddress;
			cpuHandle = other.cpuHandle;
			return *this;
		};
	};

}