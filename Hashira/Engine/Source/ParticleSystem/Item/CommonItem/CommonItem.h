#pragma once
#include "./../ParticleItem.h"
namespace Hashira {
	class CommonItem :
		public ParticleItem
	{

	public:

	private:

	public:
		CommonItem();
		virtual ~CommonItem();

		// 要素の値へのVOID型ポインタ
		virtual void* GetValue();

		//パーティクル単位のデータの更新はGPUだけど、作用するアイテムデータの更新はParticleItemが行う
		virtual void UpdateItem();

		//GPUにマッピングする際に書き込むサイズの取得
		virtual unsigned int GetElementSize();


	};

};