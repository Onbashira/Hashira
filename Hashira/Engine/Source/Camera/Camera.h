#pragma once
#include "Engine/Source/Math/Math.h"
#include "Engine/Source/Component/GameObject/GameObject.h"
#include "Engine/Source/Buffer/UploadBuffer.h"
#include "Engine/Source/Buffer/DepthStencilBuffer.h"

namespace Hashira {

	class CommandList;
	
	//!カメラのモード
	enum class CAMERA_MODE {
		Perspective,
		Orthogonal
	};

	//!シェーダーにバインドするカメラの情報
	struct CameraInfo
	{
		Matrix view;
		Matrix projection;
		Matrix invView;
		Matrix invViewProj;
		float  windowWidth;
		float  windowHeight;
	};

	class Camera :
		public GameObject
	{

	public:

	private:
		//！モード
		CAMERA_MODE _mode;

		//!Field　Of　View
		float _fov;

		//!アス比
		float _aspectRatio;
		//!ニアクリップ
		float _near;
		
		//!ファークリップ
		float _far;

		//!ウインドウ幅
		float _windowWidth;
		
		//!ウインドウ高さ
		float _windowHeight;

		//!ビューポート
		D3D12_VIEWPORT	_viewport;

		//!切り抜き矩形
		D3D12_RECT _scissorRect;

		//!射影行列
		Matrix _projection;
		//!カメラインフォメーションバッファ
		UploadBuffer<CameraInfo > _cameraMatrixBuffer;
		
		//!デプスステンシルバッファ
		DepthStencil _depthStencillRersource;

		//!カメラの情報
		CameraInfo _info;

	public:

		Camera();

		~Camera();

		/**
		* @fn
		* @brief カメラのモードを取得
		* @return モード
		*/
		CAMERA_MODE		GetMode();

		/**
		* @fn
		* @brief 射影行列の取得
		* @return 射影行列
		*/
		const Matrix&	GetProjection();

		/**
		* @fn
		* @brief 射影行列の取得
		* @return 射影行列
		*/
		const Matrix	GetViewProjection();

		/**
		* @fn
		* @brief カメラの情報の取得
		* @return カメラの情報
		*/
		CameraInfo		GetCameraInfo();

		/**
		* @fn
		* @brief カメラバッファの取得
		* @return カメラバッファへの参照
		*/
		Hashira::UploadBuffer<Hashira::CameraInfo>&  GetCameraBuffer();

		/**
		* @fn
		* @brief デプスステンシルバッファの取得
		* @return デプスステンシルバッファへの参照
		*/
		DepthStencil&	GetDepthStencil();

		/**
		* @fn
		* @brief FOVの取得
		* @return FOV
		*/
		float GetFov();

		/**
		* @fn
		* @brief アスペクト比の取得
		* @return アスペクト比
		*/
		float GetAspectratio;

		/**
		* @fn
		* @brief ニアの取得
		* @return ニア
		*/
		float GetNearClip();

		/**
		* @fn
		* @brief ファーの取得
		* @return ファー
		*/
		float GetFarClip();

		/**
		* @fn
		* @brief ビューポートの取得
		* @return ビューポートへの参照
		*/
		const D3D12_VIEWPORT& GetViewport()const;

		/**
		* @fn
		* @brief 切り抜き矩形の取得
		* @return 切り抜き矩形への参照
		*/
		const D3D12_RECT& GetScissorRect()const;

		/**
		* @fn
		* @brief バッファ及び情報の更新
		*/
		void Update()override;

		/**
		* @fn
		* @brief ダミー描画
		*/
		void Draw(std::shared_ptr<CommandList>& cmdList)override {};

		/**
		* @fn
		* @brief 入力により移動
		* @oaram[in] input　入力
		*/
		void DebugMove(InputManager& input);

		/**
		* @fn
		* @brief 入力により回転
		* @oaram[in] input　入力
		*/
		void DebugRotate(InputManager& input);

		/**
		* @fn
		* @brief モードの変更
		* @oaram[in] mode　モード
		*/
		void ChangeCameraMode(CAMERA_MODE mode);

		/**
		* @fn
		* @brief カメラの初期化
		* @oaram[in] type　モード
		* @oaram[in] width　カメラの画面幅
		* @oaram[in] height　カメラの画面高さ
		* @oaram[in] nearClip　ニア
		* @oaram[in] farClip　ファー
		* @oaram[in] position　位置
		* @oaram[in] target　ターゲット位置
		* @oaram[in] upWard　アップベクトル
		*/
		void InitializeCamera(CAMERA_MODE type, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		/**
		* @fn
		* @brief FOVベースでカメラの初期化
		* @oaram[in] fov　FOV
		* @oaram[in] width　カメラの画面幅
		* @oaram[in] height　カメラの画面高さ
		* @oaram[in] nearClip　ニア
		* @oaram[in] farClip　ファー
		* @oaram[in] position　位置
		* @oaram[in] target　ターゲット位置
		* @oaram[in] upWard　アップベクトル
		*/
		void InitializeCameraFOV(const float fov, const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

		/**
		* @fn
		* @brief デプスステンシルバッファの初期化
		* @oaram[in] depthFormat　フォーマット
		* @oaram[in] windowWidth　カメラの画面幅
		* @oaram[in] windowHeight　カメラの画面高さ
		* @return リザルト S_OKで成功
		*/
		HRESULT InitializeCameraDepthStencill(std::shared_ptr<D3D12Device>& device,DXGI_FORMAT depthFormat, unsigned int windowWidth, unsigned int windowHeight);

		/**
		* @fn
		* @brief コマンドリストにカメラ情報をセット
		* @oaram[in] list　セット先
		* @oaram[in] parameterIndex　パラメータのインデックス
		*/
		void SetCameraParameter(std::weak_ptr<CommandList> list, unsigned int parameterIndex = 0);

		/**
		* @fn
		* @brief 破棄
		*/
		void Discard();

	private:

		/**
		* @fn
		* @brief 各バッファの作成
		* @return リザルト S_OKで成功
		*/
		HRESULT CreateBuffer();

		/**
		* @fn
		* @brief 正射影カメラの初期化
		* @oaram[in] fov　FOV
		* @oaram[in] width　カメラの画面幅
		* @oaram[in] height　カメラの画面高さ
		* @oaram[in] nearClip　ニア
		* @oaram[in] farClip　ファー
		* @oaram[in] position　位置
		* @oaram[in] target　ターゲット位置
		* @oaram[in] upWard　アップベクトル
		* @return リザルト S_OKで成功
		*/

		HRESULT InitializeOrthogonal(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);
		
		/**
		* @fn
		* @brief 射影カメラの初期化
		* @oaram[in] fov　FOV
		* @oaram[in] width　カメラの画面幅
		* @oaram[in] height　カメラの画面高さ
		* @oaram[in] nearClip　ニア
		* @oaram[in] farClip　ファー
		* @oaram[in] position　位置
		* @oaram[in] target　ターゲット位置
		* @oaram[in] upWard　アップベクトル
		* @return リザルト S_OKで成功
		*/
		HRESULT initializePerspective(const float width, const float height, const float nearClip, const float farClip, const Vector3& position, const Vector3& target, const Vector3& upWard);

	};

}