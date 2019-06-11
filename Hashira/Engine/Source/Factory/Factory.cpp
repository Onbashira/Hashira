#include "Factory.h"
#include "Engine/Source/Debug/Logger.h"
#include <string>


Hashira::Factory::Factory() :
	_factory()
{
}


Hashira::Factory::~Factory()
{
	_factory.Reset();
}


HRESULT Hashira::Factory::Initialize(UINT createFlags)
{
#if defined(_DEBUG)
	// DirectX12�̃f�o�b�O���C���[��L���ɂ���
	{
		Microsoft::WRL::ComPtr<ID3D12Debug>	debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
			debugController->EnableDebugLayer();

		}
	}
	createFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif
	// DirectX12���T�|�[�g���闘�p�\�ȃn�[�h�E�F�A�A�_�v�^���擾
	auto hr = CreateDXGIFactory2(createFlags, IID_PPV_ARGS(&_factory));
	if (FAILED(hr))
		return FALSE;
	return S_OK;
}

Microsoft::WRL::ComPtr<IDXGIFactory4>& Hashira::Factory::GetFactory()
{
	return _factory;
}

void Hashira::Factory::Discard()
{
	if (_factory != nullptr) {
		_factory.Reset();
		DEBUG_LOG(std::string("Factory is Reset"));
	}
}

