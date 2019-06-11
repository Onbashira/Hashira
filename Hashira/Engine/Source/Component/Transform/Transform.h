#pragma once

namespace Hashira {

	//!オブジェクトの移動、回転情報を保持、計算するクラス
	class Transform
	{
	public:

	protected:
		//!ルートオブジェクトの位置
		Vector3 _pos;

		//!ルートオブジェクトの任意軸と回転量
		Quaternion _rotation;

		//!保持オイラー角
		Vector3 _eulerAngles;
		
		//!拡縮情報
		Vector3 _scale;

	private:

	public:

		Transform();

		virtual ~Transform();

		/**
		* @fn　位置直接設定
		* @param[in] pos 位置
		*/
		void SetPos(const Vector3 pos);

		/**
		* @fn　回転量直接設定
		* @param[in] rotation 回転
		*/
		void SetRotation(const Quaternion rotation);

		/**
		* @fn　回転量直接設定
		* @param[in] euler 回転
		*/
		void SetEulerAngles(const Vector3 euler);

		/**
		* @fn　スケール直接設定
		* @param[in] scale スケール
		*/
		void SetScale(const Vector3 scale);

		/**
		* @fn　位置の取得
		* @return 位置
		*/
		Vector3 GetPos();

		/**
		* @fn　回転の取得
		* @return 四元数
		*/
		Quaternion GetRotation();

		/**
		* @fn　回転の取得
		* @return オイラー角
		*/
		Vector3 GetEulerAngles();

		/**
		* @fn　スケール取得
		* @return スケール
		*/
		Vector3 GetScale();

		/**
		* @fn オブジェクトの移動（ローカル軸基準）
		* @param[in] velocity 変化量
		*/
		void				Translate(const Vector3& velocity);

		/**
		* @fn オブジェクトの移動（ワールド軸基準）
		* @param[in] velocity 変化量
		*/	
		void				Move(const Vector3& velocity);

		/**
		* @fn 原点中心回転
		* @param[in] rot 四元数
		*/	
		void				Rotation(const Quaternion& rot);

		/**
		* @fn ローカル中心回転
		* @param[in] rot 四元数
		*/	
		void				RotationLocalWorld(const Quaternion& rot);

		/**
		* @fn オイラー角で原点中心回転
		* @param[in] eulerAngles 変化量
		*/
		void				RotationLocalWorld(const Vector3& eulerAngles);

		/**
		* @fn オイラー角で原点中心回転
		* @param[in] eulerAngles 絶対量	
		*/
		void				RotationEulerAngles(const Vector3& eulerAngles);

		/**
		* @fn 軸と回転量で回転
		* @param[in] axis 軸
		* @param[in] rad 弧度
		*/
		void				RotationAxisAngles(const Vector3& axis, float rad);

		/**
		* @fn ポイント周りに軸と回転量で回転
		* @param[in] point 世界座標
		* @param[in] rot 回転量
		*/
		void				RotateAround(const Vector3& point, const Quaternion& rot);
		
		/**
		* @fn ポイント周りに軸と回転量で回転
		* @param[in] point 世界座標
		* @param[in] axis 軸
		* @param[in] rad 弧度
		*/
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		/**
		* @fn オブジェクトのローカルForward軸がポイントを見るように回転
		* @param[in] point ターゲット位置
		* @param[in] up アップ軸
		*/
		void				LookAt(const Vector3& point, const Vector3& up);
		
		/**
		* @fn ワールド座標で見たローカル軸の取得
		* @return ローカル軸
		*/
		OrthonormalBasis	GetLocalAxis();

		/**
		* @fn このオブジェクトのビュー行列を取得
		* @return ビュー行列
		*/
		Matrix				GetView();

		/**
		* @fn このオブジェクトのビュー行列を取得
		* @return SRT行列
		*/
		Matrix				GetSRTMatrix();

	private:

	};
}