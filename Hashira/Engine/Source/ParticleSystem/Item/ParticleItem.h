#pragma once
namespace Hashira {

	//要素ごとのアイテム
	class ParticleItem
	{
	public:

		ParticleItem() {};

		virtual ~ParticleItem() {};

		// 要素の値へのVOID型ポインタ
		virtual void* GetValue() = 0;
		
		//パーティクル単位のデータの更新はGPUだけど、エミッタ単位のデータの更新はParticleItemが行う
		virtual void UpdateItem() = 0;

		//GPUにマッピングする際に書き込むサイズの取得
		virtual unsigned int GetElementSize() = 0;

		
	};
}