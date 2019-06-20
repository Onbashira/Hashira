#include "stdafx.h"
#include "EmitterIdxTblBinary.h"


#include "Engine/Source/Resource/L0Buffer.h"
#include "Engine/Source/Device/D3D12Device.h"

Hashira::EmitterIdxTblBinary::EmitterIdxTblBinary()
{
}


Hashira::EmitterIdxTblBinary::~EmitterIdxTblBinary()
{
	this->DiscardMemory();
}

HRESULT Hashira::EmitterIdxTblBinary::Initialize(std::shared_ptr<D3D12Device> device, UINT64 memorySize)
{

	_bin = std::make_unique<L0Buffer>();
	auto hr = _bin->Initialize(device, memorySize,D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE);
#ifdef _DEBUG
	SystemLogger::GetInstance().Log(LOG_LEVEL::Debug, hr);
#endif

	return hr;
}

void Hashira::EmitterIdxTblBinary::Write(int emtIndex)
{
	auto ret = std::any_of(_emtIdxTbl.begin(), _emtIdxTbl.end(), [&, emtIndex](int  index)->bool {return (index == emtIndex); });
	
	//“¯‚¶‚à‚Ì‚ª”z—ñ“à‚É‚ ‚é‚È‚ç‚Î
	if (ret == true) 
	{
		return;
	}
	_emtIdxTbl.push_back(emtIndex);
	
	_bin->Update(_emtIdxTbl.data(), _emtIdxTbl.size() * sizeof(unsigned int), 0);


	return;

}

void Hashira::EmitterIdxTblBinary::Delete(int emtIndex)
{
}

void Hashira::EmitterIdxTblBinary::Sort()
{
	std::sort(_emtIdxTbl.begin(), _emtIdxTbl.end());

	_bin->Update(_emtIdxTbl.data(), _emtIdxTbl.size() * sizeof(unsigned int), 0);

}

void Hashira::EmitterIdxTblBinary::DiscardMemory()
{
	_bin->Discard();
	_emtIdxTbl.clear();
	_emtIdxTbl.shrink_to_fit();
}
