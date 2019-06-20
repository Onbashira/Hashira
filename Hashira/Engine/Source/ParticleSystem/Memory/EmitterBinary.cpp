#include "stdafx.h"
#include "EmitterBinary.h"
#include "Engine/Source/ParticleSystem/Emitter/Emitter.h"
#include "Engine/Source/ParticleSystem/Item/ParticleItem.h"

#include "Engine/Source/Resource/L0Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::EmitterBinary::EmitterBinary()
{
}


Hashira::EmitterBinary::~EmitterBinary()
{
	this->DiscardMemory();

}

HRESULT Hashira::EmitterBinary::Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize)
{
	_bin = std::make_unique<L0Buffer>();
	auto hr = _bin->Initialize(device, memorySize, D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;

}

void Hashira::EmitterBinary::Write(Emitter * emitter)
{

	++_emtCount;

	char* ptr = nullptr;
	//Get size of Emitter Items
	size_t binSize = emitter->GetBinSize();
	//write to temp buffer
	ptr = new char[binSize];

	EmitterBinInfo binfo;
	binfo.Begin = _offset;
	{
		size_t offset = 0;

		std::memcpy(ptr, &emitter->GetCommonItem(), sizeof(EmitterCommonItem));
		offset += sizeof(EmitterCommonItem);

		for (auto& item : emitter->GetEmitterItems()) {
			std::memcpy(ptr + offset, item->GetValue(), item->GetElementSize());
			offset += item->GetElementSize();
		}
	}

	_bin->Update(ptr, binSize, _offset);
	_offset += binSize;
	binfo.End = _offset;
	_binInfoVec.push_back(binfo);
	delete[] ptr;
}

void Hashira::EmitterBinary::Delete(int emtIndex)
{

}

void Hashira::EmitterBinary::DiscardMemory()
{
	this->_bin->Discard();
}
