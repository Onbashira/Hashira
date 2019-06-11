#pragma once
#include "../ParticleItem.h"

namespace Hashira {
	class RotateAnimItem : public ParticleItem
	{
	public:
		RotateAnimItem();
		~RotateAnimItem();

		// 要素の値へのVOID型ポインタ
		virtual void* GetValue()override;

		//パーティクル単位のデータの更新はGPUだけど、エミッタ単位のデータの更新はParticleItemが行う
		virtual void UpdateItem() = 0;

		//GPUにマッピングする際に書き込むサイズの取得
		virtual unsigned int GetElementSize()override;
	};

}