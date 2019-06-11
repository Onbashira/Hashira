#pragma once

namespace Hashira {

	//!�I�u�W�F�N�g�̈ړ��A��]����ێ��A�v�Z����N���X
	class Transform
	{
	public:

	protected:
		//!���[�g�I�u�W�F�N�g�̈ʒu
		Vector3 _pos;

		//!���[�g�I�u�W�F�N�g�̔C�ӎ��Ɖ�]��
		Quaternion _rotation;

		//!�ێ��I�C���[�p
		Vector3 _eulerAngles;
		
		//!�g�k���
		Vector3 _scale;

	private:

	public:

		Transform();

		virtual ~Transform();

		/**
		* @fn�@�ʒu���ڐݒ�
		* @param[in] pos �ʒu
		*/
		void SetPos(const Vector3 pos);

		/**
		* @fn�@��]�ʒ��ڐݒ�
		* @param[in] rotation ��]
		*/
		void SetRotation(const Quaternion rotation);

		/**
		* @fn�@��]�ʒ��ڐݒ�
		* @param[in] euler ��]
		*/
		void SetEulerAngles(const Vector3 euler);

		/**
		* @fn�@�X�P�[�����ڐݒ�
		* @param[in] scale �X�P�[��
		*/
		void SetScale(const Vector3 scale);

		/**
		* @fn�@�ʒu�̎擾
		* @return �ʒu
		*/
		Vector3 GetPos();

		/**
		* @fn�@��]�̎擾
		* @return �l����
		*/
		Quaternion GetRotation();

		/**
		* @fn�@��]�̎擾
		* @return �I�C���[�p
		*/
		Vector3 GetEulerAngles();

		/**
		* @fn�@�X�P�[���擾
		* @return �X�P�[��
		*/
		Vector3 GetScale();

		/**
		* @fn �I�u�W�F�N�g�̈ړ��i���[�J������j
		* @param[in] velocity �ω���
		*/
		void				Translate(const Vector3& velocity);

		/**
		* @fn �I�u�W�F�N�g�̈ړ��i���[���h����j
		* @param[in] velocity �ω���
		*/	
		void				Move(const Vector3& velocity);

		/**
		* @fn ���_���S��]
		* @param[in] rot �l����
		*/	
		void				Rotation(const Quaternion& rot);

		/**
		* @fn ���[�J�����S��]
		* @param[in] rot �l����
		*/	
		void				RotationLocalWorld(const Quaternion& rot);

		/**
		* @fn �I�C���[�p�Ō��_���S��]
		* @param[in] eulerAngles �ω���
		*/
		void				RotationLocalWorld(const Vector3& eulerAngles);

		/**
		* @fn �I�C���[�p�Ō��_���S��]
		* @param[in] eulerAngles ��Η�	
		*/
		void				RotationEulerAngles(const Vector3& eulerAngles);

		/**
		* @fn ���Ɖ�]�ʂŉ�]
		* @param[in] axis ��
		* @param[in] rad �ʓx
		*/
		void				RotationAxisAngles(const Vector3& axis, float rad);

		/**
		* @fn �|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		* @param[in] point ���E���W
		* @param[in] rot ��]��
		*/
		void				RotateAround(const Vector3& point, const Quaternion& rot);
		
		/**
		* @fn �|�C���g����Ɏ��Ɖ�]�ʂŉ�]
		* @param[in] point ���E���W
		* @param[in] axis ��
		* @param[in] rad �ʓx
		*/
		void				RotateAround(const Vector3& point, const Vector3& axis, float rad);

		/**
		* @fn �I�u�W�F�N�g�̃��[�J��Forward�����|�C���g������悤�ɉ�]
		* @param[in] point �^�[�Q�b�g�ʒu
		* @param[in] up �A�b�v��
		*/
		void				LookAt(const Vector3& point, const Vector3& up);
		
		/**
		* @fn ���[���h���W�Ō������[�J�����̎擾
		* @return ���[�J����
		*/
		OrthonormalBasis	GetLocalAxis();

		/**
		* @fn ���̃I�u�W�F�N�g�̃r���[�s����擾
		* @return �r���[�s��
		*/
		Matrix				GetView();

		/**
		* @fn ���̃I�u�W�F�N�g�̃r���[�s����擾
		* @return SRT�s��
		*/
		Matrix				GetSRTMatrix();

	private:

	};
}