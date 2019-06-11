#pragma once

namespace Hashira {

	class Material
	{
	public:

	private:
		//! べ－スカラー
		Vector3 _baseColor;
		//!アンビエントカラー
		Vector3 _ambient;
		//!鏡面反射カラー
		Vector3 _specular;
		//!鏡面反射パワー
		float _specularPower;
		//ざらつきパラメーター 0.0f～1.0f
		float _roughness;
		//金属度合いパラメーター 0.0f～1.0f
		float _metalness;

	public:

		Material() {};

		~Material() {};

		void SetBaseColor(const Vector3& color);

		void SetSpecular(const Vector3& color, float power = 1.0f);

		void SetAmbient(const Vector3& ambient);

		void SetRoughness(float roughness);

		void SetMetalness(float metalness);

		Vector3& GetBaseColor();

		Vector3& GetAmbient();

		float& GetRoughness();

		float& GetMetalness();

	private:

	};
}
