#pragma once
#include "../ParticleItem.h"
#include "Engine/Source/Component/Transform/Transform.h"
namespace Hashira {

	class Transform;

	//エミッタの位置、回転、スケールをサポートするデフォルトアイテム
	class TransformItem : public ParticleItem
	{
	private:

		Transform _transform;

	public:
		TransformItem();
		~TransformItem();

		// 要素の値へのVOID型ポインタ
		virtual void* GetValue();

		//パーティクル単位のデータの更新はGPUだけど、エミッタ単位のデータの更新はParticleItemが行う
		virtual void UpdateItem();

		//GPUにマッピングする際に書き込むサイズの取得
		virtual unsigned int GetElementSize();

	private:
	};

}