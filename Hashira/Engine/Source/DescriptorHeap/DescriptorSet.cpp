#include "stdafx.h"
#include "DescriptorSet.h"


Hashira::DescriptorSystem::DescriptorSet::DescriptorSet()
{
}


Hashira::DescriptorSystem::DescriptorSet::~DescriptorSet()
{
}

void Hashira::DescriptorSystem::DescriptorSet::Reset()
{
	_vsCbv.Reset();
	_vsSrv.Reset();
	_vsSampler.Reset();
	_psCbv.Reset();
	_psSrv.Reset();
	_psSampler.Reset();
	_psUav.Reset();
	_gsCbv.Reset();
	_gsSrv.Reset();
	_gsSampler.Reset();
	_hsCbv.Reset();
	_hsSrv.Reset();
	_hsSampler.Reset();
	_dsCbv.Reset();
	_dsSrv.Reset();
	_dsSampler.Reset();
	_csCbv.Reset();
	_csSrv.Reset();
	_csSampler.Reset();
	_csUav.Reset();
}

void Hashira::DescriptorSystem::DescriptorSet::SetVsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSystem::DescriptorSet::SetVsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetVsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsSampler.SetHandles(index, handle);
}

void Hashira::DescriptorSystem::DescriptorSet::SetPsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSystem::DescriptorSet::SetPsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetPsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetPsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psUav.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetGsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSystem::DescriptorSet::SetGsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetGsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetHsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetHsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetHsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetDsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetDsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetDsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetCsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetCsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetCsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSystem::DescriptorSet::SetCsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csUav.SetHandles(index, handle);

}
