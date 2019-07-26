#include "stdafx.h"
#include "DescriptorSet.h"


Hashira::DescriptorSet::DescriptorSet()
{
}


Hashira::DescriptorSet::~DescriptorSet()
{
}

void Hashira::DescriptorSet::Reset()
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

void Hashira::DescriptorSet::SetVsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSet::SetVsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetVsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_vsSampler.SetHandles(index, handle);
}

void Hashira::DescriptorSet::SetPsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSet::SetPsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetPsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetPsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_psUav.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetGsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsCbv.SetHandles(index, handle);
}

void Hashira::DescriptorSet::SetGsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetGsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_gsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetHsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetHsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetHsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_hsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetDsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetDsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetDsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_dsSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetCsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csCbv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetCsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csSrv.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetCsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csSampler.SetHandles(index, handle);

}

void Hashira::DescriptorSet::SetCsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE & handle)
{
	_csUav.SetHandles(index, handle);

}
