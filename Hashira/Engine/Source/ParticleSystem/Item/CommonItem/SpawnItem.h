#pragma once
#include "../ParticleItem.h"

namespace Hashira {
	class SpawnItem :
		public ParticleItem
	{
	public:
		SpawnItem();
		virtual ~SpawnItem();

		// 要素の値へのVOID型ポインタ
		virtual void* GetValue();

		//パーティクル単位のデータの更新はGPUだけど、エミッタ単位のデータの更新はParticleItemが行う
		virtual void UpdateItem();

		//GPUにマッピングする際に書き込むサイズの取得
		virtual unsigned int GetElementSize();
	private:
	};

}