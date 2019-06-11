#pragma once
namespace Hashira {

	class ParticleItem;
	struct GPUEmitterHeader;

	//エミッタ共通のアイテム(CPU側）
	struct CPUEmitterHeader 
	{
		//エミッタ領域のヘッドアドレス
		unsigned int EmitterBinHead = 0;
		//エミッタ領域のサイズ
		unsigned int EmmiterBinSize = 0;
		//パーティクルバイナリのヘッダアドレス
		unsigned int ParticleBinHead = 0;
		//パーティクルバイナリのサイズ
		unsigned int ParticleBinSize = 0;
		//パーティクルインデックス保持領域のインデックス番号
		unsigned int ParticleIdxHead = 0;
		//パーティクルのサイズ
		unsigned int ParticleSize = 0;

		CPUEmitterHeader() : EmitterBinHead(0), EmmiterBinSize(0),
			ParticleBinHead(0), ParticleIdxHead(0), ParticleSize(0) {};

		GPUEmitterHeader CreateGPUEmitterHeader()
		{
			return GPUEmitterHeader(EmitterBinHead,ParticleBinHead,ParticleIdxHead,ParticleSize );
		};

		~CPUEmitterHeader() {};
	};


	//GPUに投げる際にいったんこの形式にコンバートする
	struct GPUEmitterHeader 
	{
		//エミッタの領域のヘッドアドレス
		unsigned int EmitterBinHead = 0;
		//パーティクルバイナリのヘッダアドレス
		unsigned int ParticleBinHead = 0;
		//パーティクルインデックス保持領域のインデックス番号
		unsigned int ParticleIdxHead = 0;
		//パーティクルのサイズ
		unsigned int ParticleSize = 0;

		GPUEmitterHeader() : EmitterBinHead(),
			ParticleBinHead(), ParticleIdxHead(), ParticleSize() {};

		GPUEmitterHeader(unsigned int emtBinHead,unsigned int ptBinHead,unsigned int ptIdxHead,unsigned int ptSize) : 
			EmitterBinHead(emtBinHead),ParticleBinHead(ptBinHead), ParticleIdxHead(ptIdxHead), ParticleSize(ptSize) {};
		~GPUEmitterHeader() {};
	};

	//エミッタデータ（GPU内でのみ読み書きされるメモリのフォーマット
	struct EmitterParam 
	{
		//メモリ位置
		unsigned int SpawnHead = 0;
		//スポーン最大数
		unsigned int SpawnMax = 0;
		//パーティクル数
		unsigned int ParticleNum = 0;
	};

	enum EmitterState 
	{
		EMITTER_STATE_START = 0,
		EMITTER_STATE_UPDATE = 0x0001 ,
		EMITTER_STATE_DEAD = 0x0002,
	};

	enum EmitterActiveItem 
	{
		EMITTER_ACITVE_ITEM_NONE = 0x0000,
		EMITTER_ACITVE_ITEM_SHAPE_3D = 0x0001,
		EMITTER_ACITVE_ITEM_PARENT_OPTION = 0x0002,
		EMITTER_ACITVE_ITEM_TURBULEANCE = 0x0004,
		EMITTER_ACITVE_ITEM_VELOCITY = 0x0008,
		EMITTER_ACITVE_ITEM_SCALE_ANIM = 0x0010,
		EMITTER_ACITVE_ITEM_ROTATE_ANIM = 0x0020,
		EMITTER_ACITVE_ITEM_NOISE = 0x0040,
		EMITTER_ACITVE_ITEM_BLINK = 0x0080,
		EMITTER_ACITVE_ITEM_BILLBORD_3D = 0x0100,
		EMITTER_ACITVE_ITEM_RIBBON = 0x0200,
		EMITTER_ACITVE_ITEM_FLOW_MAP = 0x0400,
		EMITTER_ACITVE_ITEM_UV_SEQUENCE = 0x0800,
		EMITTER_ACITVE_ITEM_LIFE = 0x1000,
		EMITTER_ACITVE_ITEM_RGB_FILTER = 0x2000,
		EMITTER_ACITVE_ITEM_RGB_WATER = 0x4000,

	};

	enum EmitterTransformMode
	{
		EMITTER_TRANFORM_MODE_TRANFORM_LOCAL = 0x0001,
		EMITTER_TRANFORM_MODE_TRANFORM_SCALE_ANIM_LOCAL = 0x0002,
		EMITTER_TRANFORM_MODE_TRANFORM_ROTATE_ANIM_LOCAL = 0x0004,

	};

	struct EmitterCommonItem 
	{
		//エミッタの状態がアップデート可能か
		EmitterState State;
		//エミッタがもつ基礎アイテムアクティブフラグ
		EmitterActiveItem ActiveItemState;
		//ローカル座標で変形かワールド座標で変形か
		EmitterTransformMode TransformMode;
		//エミッタの位置
		Vector3 Pos;
		//エミッタのスケール
		Vector3 Scale;
		//エミッタの回転度合い（Quaternion
		Quaternion Rotate;

	};

	class Emitter
	{

	public:

	protected:

		//エミッタアイテム部のサイズ（Byte) (共通アイテム部サイズ　∔　ItemのGPU書き込み総サイズ
		unsigned int _dataSize;

		//エミッタパラメータ
		EmitterParam _emitterData;

		//共通アイテム
		EmitterCommonItem _emitterCommonItem;

		//アイテムアップデータ
		std::function<void()> _updater;

		//アイテム配列
		std::vector < std::unique_ptr<ParticleItem> >_items;

		//子エミッタ
		std::vector<Emitter*> _childEmitters;

		//エミッタの名前
		String _emtName;

	public:
		
		Emitter();

		~Emitter();

		//各アイテムのアップデート
		void UpdateItems();

		//アイテムアップデータの変更
		void SetCustomUpdater(std::function<void(void)> function);

		//エミッタの情報のセット
		void SetEmitterParam(const EmitterParam& emitterData);

		void SetName(String& name);

		//アイテム部サイズの取得
		const unsigned int& GetBinSize();

		//共通アイテムの取得
		EmitterCommonItem& GetCommonItem();
		
		//エミッタアイテム配列の取得
		std::vector < std::unique_ptr<ParticleItem> >& GetEmitterItems();

		//子エミッタへのポインタ配列
		std::vector<Emitter*>& GetChildren();

		template <class T>
		Emitter& AddItems()
		{
			_items.push_back(std::make_unique<T>());
			_dataSize += static_cast<unsigned int>(sizeof(T));
			return *this;
		};

		//データの破棄
		void Discard();

	private:
			

	};

}