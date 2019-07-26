#pragma once



namespace Hashira {


		constexpr Uint32 CbvMaxCount = 16;
		constexpr Uint32 SrvMaxCount = 48;
		constexpr Uint32 UavMaxCount = 16;
		constexpr Uint32 SamplerMaxCount = 16;


		class DescriptorSet
		{
		
		public:
		
		private:
			template <Uint32 Num>
			struct Handles
			{
				D3D12_CPU_DESCRIPTOR_HANDLE cpuHandles[Num];
				Uint32 maxCount;
				void Reset()
				{
					memset(cpuHandles, 0,sizeof(cpuHandles));
					maxCount = 0;
				}

				void SetHandles(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle)
				{
					assert(index < Num);
					cpuHandles[index] = handle;
					maxCount = Max(maxCount, index + 1);

				}
			};

			Handles<CbvMaxCount>		_vsCbv;
			Handles<SrvMaxCount>		_vsSrv;
			Handles<SamplerMaxCount>	_vsSampler;
			Handles<CbvMaxCount>		_psCbv;
			Handles<SrvMaxCount>		_psSrv;
			Handles<SamplerMaxCount>	_psSampler;
			Handles<UavMaxCount>		_psUav;
			Handles<CbvMaxCount>		_gsCbv;
			Handles<SrvMaxCount>		_gsSrv;
			Handles<SamplerMaxCount>	_gsSampler;
			Handles<CbvMaxCount>		_hsCbv;
			Handles<SrvMaxCount>		_hsSrv;
			Handles<SamplerMaxCount>	_hsSampler;
			Handles<CbvMaxCount>		_dsCbv;
			Handles<SrvMaxCount>		_dsSrv;
			Handles<SamplerMaxCount>	_dsSampler;
			Handles<CbvMaxCount>		_csCbv;
			Handles<SrvMaxCount>		_csSrv;
			Handles<SamplerMaxCount>	_csSampler;
			Handles<UavMaxCount>		_csUav;
		public:						   
			
			DescriptorSet();
			
			~DescriptorSet();

			void Reset();
		
			void SetVsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetVsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetVsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetPsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetPsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetPsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetPsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetGsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetGsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetGsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetHsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetHsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetHsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetDsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetDsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetDsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetCsCbv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetCsSrv(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetCsSampler(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);
			void SetCsUav(Uint32 index, const D3D12_CPU_DESCRIPTOR_HANDLE& handle);

			// getter
			const Handles<CbvMaxCount>& GetVsCbv() const { return _vsCbv; }
			const Handles<SrvMaxCount>& GetVsSrv() const { return _vsSrv; }
			const Handles<SamplerMaxCount>& GetVsSampler() const { return _vsSampler; }
			const Handles<CbvMaxCount>& GetPsCbv() const { return _psCbv; }
			const Handles<SrvMaxCount>& GetPsSrv() const { return _psSrv; }
			const Handles<SamplerMaxCount>& GetPsSampler() const { return _psSampler; }
			const Handles<UavMaxCount>& GetPsUav() const { return _psUav; }
			const Handles<CbvMaxCount>& GetGsCbv() const { return _gsCbv; }
			const Handles<SrvMaxCount>& GetGsSrv() const { return _gsSrv; }
			const Handles<SamplerMaxCount>& GetGsSampler() const { return _gsSampler; }
			const Handles<CbvMaxCount>& GetHsCbv() const { return _hsCbv; }
			const Handles<SrvMaxCount>& GetHsSrv() const { return _hsSrv; }
			const Handles<SamplerMaxCount>& GetHsSampler() const { return _hsSampler; }
			const Handles<CbvMaxCount>& GetDsCbv() const { return _dsCbv; }
			const Handles<SrvMaxCount>& GetDsSrv() const { return _dsSrv; }
			const Handles<SamplerMaxCount>& GetDsSampler() const { return _dsSampler; }
			const Handles<CbvMaxCount>& GetCsCbv() const { return _csCbv; }
			const Handles<SrvMaxCount>& GetCsSrv() const { return _csSrv; }
			const Handles<SamplerMaxCount>& GetCsSampler() const { return _csSampler; }
			const Handles<UavMaxCount>& GetCsUav() const { return _csUav; }


		private:
		
		};

}