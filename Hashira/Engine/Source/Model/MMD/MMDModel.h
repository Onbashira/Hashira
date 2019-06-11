#pragma once
#include "Engine/Source/Model/Model/ModelObject.h"

namespace K3D {

	class GraphicsCommandList;
	class PipelineStateObject;
	class ModelConverter;

	struct MMDRendererMaterial
	{
		//拡散反射光 w = alpha
		Vector4 diffuse;
		//鏡面反射光	w = pow
		Vector4 specular;
		//環境光
		Vector3 ambient;
		//自己発光色
		Vector3 emissive;
		//スフィアマップのブレンディングタイプ
		int	sphereBlendType;
	};

	//ゲーム本編で使うモデル
	class MMDModel :
		public ModelObject
	{
		friend class ModelConverter;
		friend class ModelPool;

		enum SHADER_ROOT_PARAMATER_INDEX {
			SHADER_ROOT_PARAMATER_INDEX_CAMERA = 0,
			SHADER_ROOT_PARAMATER_INDEX_TRANSFORM,
			SHADER_ROOT_PARAMATER_INDEX_MATERIAL,
			SHADER_ROOT_PARAMATER_INDEX_TEXTURE,
			SHADER_ROOT_PARAMATER_INDEX_TOON_MAP,
			SHADER_ROOT_PARAMATER_INDEX_SPHERE_TEXTURE,
		};
	private:
		
	protected:
		//モデルのオリジナルデータ
		std::weak_ptr<MMDModelResourceData>	_resourceData;
		//アニメーターコントローラー
		Animator							_animator;

	public:

	private:
		//BDEF1計算
		Matrix CulcBDEF1(K3D::WeightDeform& deform);
		//BDEF2計算
		Matrix CulcBDEF2(WeightDeform& deform);
		//BDEF4計算
		Matrix CulcBDEF4(WeightDeform& deform);
		//SDEF計算
		std::pair<Vector3, float> CulcSDEF(WeightDeform& deform);
		//QDEF計算
		Matrix CulcQDEF(WeightDeform& deform);
		//SDEF重み計算
		std::pair<float, float> CulcSDEFWaight(Vector3 r0, Vector3 r1);

	protected:
		//トランスフォームアップデート
		void	TransformUpdate();
		//アニメーションのアップデート
		void	AnimationUpdate();
		//ボーンのアップデート
		void	SkeltonReset();
		//アニメーションデーターの登録とロード
		void	SetAnimationData(std::string motionPath);
		//バンドルへの登録
		virtual void RegisterToBundle()override;

	public:
		//トランスフォームのアップデート
		virtual void Update()override;
		//リストにDrawコマンドを積み込む(ジオメトリレンダリング）
		virtual void Draw()override;

		virtual Animator& Animator();

		MMDModel();
		virtual ~MMDModel();
	};
}