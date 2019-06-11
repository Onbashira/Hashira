#pragma once
#include "Engine/Source/Utility/D3D12Common.h"
#include "Engine/Source/LifetimedMemory/LifetimedMemory.h"
#include <tuple>
#include <string>


namespace Hashira {

	class CommandList;
	class D3D12Device;
	class Resource
	{

	private:

	protected:

		//!現在のリソースステート
		D3D12_RESOURCE_STATES _currentResourceState;

		//!マップされたポインタ
		UCHAR* _pDst;

		//!クリアバリュー
		D3D12_CLEAR_VALUE _clearValue;

		//!レジスタ番号
		unsigned int _shaderRegisterNumber;

		//!リソース本体
		Microsoft::WRL::ComPtr<ID3D12Resource> _resource;

		//!名前
		std::string _name;

	private:

	public:


		Resource(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		Resource(const Resource& other);

		Resource(Resource&& value);

		Resource& operator= (const Resource& value);

		Resource& operator= (Resource&& value);

		virtual ~Resource();

		/**
		* @fn
		* @brief 未初期化のバッファのユニークなポインタの取得
		* @return ユニークポインタ
		*/
		static  std::unique_ptr<Hashira::Resource> CreateUnique();

		/**
		* @fn
		* @brief 未初期化のバッファの強参照の取得
		* @return 強参照
		*/
		static  std::shared_ptr<Hashira::Resource> CreateShared();

		/**
		* @fn
		* @brief 未初期化のバッファの生存保証つき強参照の取得
		* @return 生存保証つき強参照
		*/
		static  LifetimedShared_Ptr<Resource> CreateLifetimedShared();

		/**
		* @fn
		* @brief バッファの作成
		* @param[in] heapProps プロパティ
		* @param[in] flags ヒープフラグ
		* @param[in] resourceDesc バッファのデスクリプション
		* @param[in] state 初期ステート
		* @param[in] clearValue クリアバリュー
		* @return リザルト
		*/
		virtual HRESULT Initialize(const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		/**
		* @fn
		* @brief バッファの作成
		* @param[in] device デバイス
		* @param[in] heapProps プロパティ
		* @param[in] flags ヒープフラグ
		* @param[in] resourceDesc バッファのデスクリプション
		* @param[in] state 初期ステート
		* @param[in] clearValue クリアバリュー
		* @return リザルト
		*/
		virtual HRESULT Initialize(std::shared_ptr<D3D12Device>& device, const D3D12_HEAP_PROPERTIES& heapProps, const D3D12_HEAP_FLAGS& flags, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_RESOURCE_STATES& state, D3D12_CLEAR_VALUE* clearValue = nullptr);

		/**
		* @fn
		* @brief マッピング
		* @param[in] subResource サブリソース
		* @param[in] readRange 読み込みレンジ
		* @return リザルト
		*/
		virtual HRESULT Map(UINT subResource, D3D12_RANGE* readRange);

		/**
		* @fn
		* @brief マッピング解除
		* @param[in] subResource サブリソース
		* @param[in] writtenRange 書き込みレンジ
		*/
		virtual void Unmap(UINT subResource, D3D12_RANGE* writtenRange);

		/**
		* @fn
		* @brief マッピングされたポインタの取得
		* @return バッファにマッピングされたポインタ
		*/
		UCHAR* GetMappedPointer();

		/**
		* @fn
		* @brief バッファ破棄
		*/
		void Discard();

		/**
		* @fn
		* @brief バッファ更新・書き込み
		* @param[in] pSrc ソースへのポインタ
		* @param[in] size サイズ
		* @param[in] dstOffset オフセット
		*/
		virtual void Update(const void* pSrc, ULONG64 size, const size_t dstOffset);

		/**
		* @fn
		* @brief バッファ更新・書き込み
		* @param[in] pSrc 書き込み先へのポインタ
		* @param[in] size サイズ
		* @param[in] dstOffset オフセット
		*/
		virtual void Read(void* pDstBuffer, ULONG64 readSize, const unsigned int dstOffsest = 0);

		/**
		* @fn
		* @brief バッファ更新・書き込み
		* @param[in] pSrc ソースへのポインタ
		* @param[in] size サイズ
		* @param[in] dstOffset オフセット
		* @param[in] readRange 読み込みレンジ
		* @param[in] writtenRange 書き込みレンジ
		*/
		void Update(const void* pSrc, ULONG64 size, const UINT dstOffset, UINT subResource, D3D12_RANGE* readRange, D3D12_RANGE* writtenRange);

		/**
		* @fn
		* @brief 256バイトアライメントしてバッファ更新・書き込み
		* @param[in] pSrc 書き込み先へのポインタ
		* @param[in] elementCount １要素サイズ
		* @param[in] elementCount 要素数
		*/
		void Alignment256ByteUpdate(const void* pSrc, unsigned int elementSize, unsigned int elementCount = 1);

		/**
		* @fn
		* @brief バッファのフェッチ
		* @return バッファへの参照
		*/
		Microsoft::WRL::ComPtr<ID3D12Resource>& GetResource();

		/**
		* @fn
		* @brief ヒープの情報のフェッチ
		* @return （プロパティ、フラグ）
		*/
		const std::tuple<D3D12_HEAP_PROPERTIES, D3D12_HEAP_FLAGS> GetHeapPropaties();

		/**
		* @fn
		* @brief バッファのデスクリプションのフェッチ
		* @return デスクリプション
		*/
		const D3D12_RESOURCE_DESC GetResourceDesc();

		/**
		* @fn
		* @brief バッファステートのフェッチ
		* @return ステート
		*/
		const D3D12_RESOURCE_STATES& GetResourceState();

		/**
		* @fn
		* @brief バッファステートのセット
		* @param[in] state バッファステート
		*/
		void SetResourceState(D3D12_RESOURCE_STATES state);

		/**
		* @fn
		* @brief バッファステートの遷移
		* @param[in] list コマンドリストの参照
		* @param[in] nextState バッファステート
		* @return リザルト
		*/
		HRESULT	ResourceTransition(CommandList* list, D3D12_RESOURCE_STATES nextState);

		/**
		* @fn
		* @brief バッファステートの遷移
		* @param[in] list コマンドリストへの弱参照
		* @param[in] nextState バッファステート
		* @return リザルト
		*/
		HRESULT	ResourceTransition(std::weak_ptr<CommandList> list, D3D12_RESOURCE_STATES nextState);

		/**
		* @fn
		* @brief クリアバリューのフェッチ
		* @return クリアバリュー
		*/
		D3D12_CLEAR_VALUE GetClearValue();

		/**
		* @fn
		* @brief リソースの名前の設定
		* @param[in] name 名前
		*/
		void SetName(std::string name);

		/**
		* @fn
		* @brief レジスタ番号の設定
		* @param[in] number レジスタ番号
		*/
		void RegisterShaderSlot(unsigned int number);


	protected:

		Resource();

	private:


	};
}