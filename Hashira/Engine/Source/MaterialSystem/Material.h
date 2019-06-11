#pragma once

namespace Hashira {

	class Material
	{
	public:

	private:
		//! �ׁ|�X�J���[
		Vector3 _baseColor;
		//!�A���r�G���g�J���[
		Vector3 _ambient;
		//!���ʔ��˃J���[
		Vector3 _specular;
		//!���ʔ��˃p���[
		float _specularPower;
		//������p�����[�^�[ 0.0f�`1.0f
		float _roughness;
		//�����x�����p�����[�^�[ 0.0f�`1.0f
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
