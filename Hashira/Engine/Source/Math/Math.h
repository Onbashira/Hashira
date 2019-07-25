#pragma once
#include <comdef.h>
#include <DirectXMath.h>

namespace {
	//--------------------------------------------------------------------------------------------------
	//Constant Variables
	//--------------------------------------------------------------------------------------------------
	static constexpr float F_PI = 3.1415926535897932384626433832795f;		//	 ��
	static constexpr float F_2PI = 6.283185307179586476925286766559f;		//	 2��
	static constexpr float F_1DIVPI = 0.31830988618379067153776752674503f;  //	 1/��
	static constexpr float F_PIDIV2 = 1.5707963267948966192313216916398f;   //	 ��/2
	static constexpr float F_PIDIV3 = 1.0471975511965977461542144610932f;   //	 ��/3
	static constexpr float F_PIDIV4 = 0.78539816339744830961566084581988f;  //	 ��/4
	static constexpr float F_PIDIV6 = 0.52359877559829887307710723054658f;  //	 ��/6
	static constexpr float F_EPSILON = 1.192092896e-07f;                    //	 �}�V���C�v�V����(float)

	static constexpr double D_PI = 3.1415926535897932384626433832795;		//	 ��
	static constexpr double D_2PI = 6.283185307179586476925286766559;		//	 2��
	static constexpr double D_1DVIPI = 0.31830988618379067153776752674503;	//	 1/��
	static constexpr double D_PIDIV2 = 1.5707963267948966192313216916398;	//	 ��/2
	static constexpr double D_PIDIV3 = 1.0471975511965977461542144610932;	//	 ��/3
	static constexpr double D_PIDIV4 = 0.78539816339744830961566084581988;	//	 ��/4
	static constexpr double D_PIDIV6 = 0.52359877559829887307710723054658;	//	 ��/6
	static constexpr double D_EPSILON = 2.2204460492503131e-016;			//	 �}�V���C�v�V����(double)

	static constexpr float ONB_EPSILON = 0.01f;                             // ���K�����K����Z�o����ۂɗp����C�v�V�����l

	static constexpr float RECIP_PI = F_1DIVPI;                             
	static constexpr float RECIP_PI2 = 0.15915494f;
	static constexpr float LOG2 = 1.442695f;
	static constexpr float GAMMA_FACTOR = 2.2f;


	//��̒l����傫���l�����߂�
	template< typename T >inline T Max(const T& a, const T& b)
	{
		return (a > b) ? a : b;
	};

	//��̒l���珬�����l�����߂�
	template< typename T >inline T Min(const T& a, const T& b)
	{
		return (a < b) ? a : b;
	};

	//�l���ő�l�ƍŏ��l�͈͓̔��ɃN�����s���O����
	template< typename T >inline T Clamp(const T& value, const T& mini, const T& maxi)
	{
		return Max(mini, Min(maxi, value));
	};

	//�^����ꂽ�l��0�`1�ɐ�������
	template< typename T >inline T Saturate(const T& value)
	{
		return Clamp(value, static_cast<T>(0), static_cast<T>(1));
	};

	//�����t���ǂ����𔻒�
	template< typename T >inline T Sign(const T& value) {
		return (value < static_cast<T>(0)) ? static_cast<T>(-1) : static_cast<T>(1);
	};

}




namespace Hashira {

	//--------------------------------------------------------------------------------------------------
	// Forward Declarations
	//--------------------------------------------------------------------------------------------------
	struct Vector2;
	struct Vector3;
	struct Vector4;
	struct Quaternion;
	struct Matrix;

	//--------------------------
	//Utility
	//--------------------------

	//�p�x�ϊ��@�x�����W�A��
	float		DegToRad(float deg);
	//�p�x�ϊ��@�x�����W�A��
	float		RadToDeg(float rad);

	//�l���[���ł��邩�ǂ���
	bool		IsZero(float value);
	bool		IsZero(double value);

	//�l���������ǂ���
	bool		IsEqual(float a, float b);
	bool		IsEqual(double a, double b);

	//�񐔂��ǂ���
	bool		IsNan(float value);

	//�����傩�ǂ���
	bool		IsInf(float value);

	//�K��
	unsigned int Fact(unsigned int value);

	//��d�K��
	unsigned int DoubleFact(unsigned int value);

	//�o�[���X�^�C��
	float		Bernstein(unsigned int n, unsigned int i, float t);

	//�񍀌W��
	float		Binormal(unsigned int n, unsigned int k);

	//����
	unsigned int Perm(unsigned int n, unsigned int r);
	//�g�ݍ��킹
	unsigned int Comb(unsigned int n, unsigned int r);

	//������Ƒ���SQRT
	float		QuickSqrt(float value);

	//�t���l����
	float		Fresnel(float n1, float n2, float cosTheta);
	double		Fresnel(double n1, double n2, double cosTheta);

	//���`���
	float		Lerp(float a, float b, float amount);
	double		Lerp(double a, double b, double amount);

	inline float pow2(float x) { return x * x; }

	inline double pow2(double x) { return x * x; }

	inline float pow3(float x) { return x * x*x; }

	inline double pow3(double x) { return x * x*x; }

	inline float pow4(float x) { return x * x*x*x; }

	inline double pow4(double x) { return x * x*x*x; }

	inline float pow5(float x) { return x * x*x*x*x; }

	inline double pow5(double x) { return x * x*x*x*x; }

	inline float clamp(float x, float a, float b) { return x < a ? a : x > b ? b : x; }

	inline double clamp(double x, double a, double b) { return x < a ? a : x > b ? b : x; }

	inline float saturate(float x) { return x < 0.f ? 0.f : x > 1.f ? 1.f : x; }

	inline double saturate(double x) { return x < 0.f ? 0.f : x > 1.f ? 1.f : x; }

	inline float recip(float x) { return 1.f / x; }

	inline double recip(double x) { return 1.f / x; }

	inline float mix(float a, float b, float t) { return a * (1.f - t) + b * t; /* return a + (b-a) * t; */ }
	
	inline double mix(double a, double b, double t) { return a * (1.f - t) + b * t; /* return a + (b-a) * t; */ }

	inline float step(float edge, float x) { return (x < edge) ? 0.f : 1.f; }

	inline double step(double edge, double x) { return (x < edge) ? 0.f : 1.f; }

	inline float smoothstep(float a, float b, float t) { if (a >= b) return 0.f; float x = saturate((t - a) / (b - a)); return x * x*(3.f - 2.f * t); }

	inline double smoothstep(double a, double b, double t) { if (a >= b) return 0.f; double x = saturate((t - a) / (b - a)); return x * x*(3.f - 2.f * t); }

	inline float radians(float deg) { return (deg / 180.f)*F_PI; }

	inline double radians(double deg) { return (deg / 180.f)*F_PI; }

	inline float degrees(float rad) { return (rad / F_PI) * 180.f; }

	inline double degrees(double rad) { return (rad / F_PI) * 180.f; }

	//--------------------------
	//Vector2
	//--------------------------
	struct  Vector2 {
		//==================================
		//�t�����h
		//==================================
		friend Vector2 operator* (float scalar, const Vector2& value);
	public:
		//==================================
		//�����o
		//==================================
		float x;
		float y;

		static const Vector2 up;
		static const Vector2 left;
		static const Vector2 down;
		static const Vector2 right;
		static const Vector2 zero;
		static const Vector2 one;

	private:
		//==================================
		//���\�b�h
		//==================================
	public:
		Vector2();
		Vector2(const float x, const float y);
		Vector2(const Vector2& other);
		Vector2(Vector2&& other);
		~Vector2();

		Vector2& operator = (const Vector2& value);
		Vector2& operator = (Vector2&& value);
		Vector2& operator +=(const Vector2& value);
		Vector2& operator -=(const Vector2& value);
		Vector2& operator *=(float scalar);
		Vector2& operator /=(float scalar);
		Vector2  operator + ()const;
		Vector2	 operator - ()const;
		Vector2	 operator + (const Vector2& value)const;
		Vector2	 operator - (const Vector2& value)const;
		Vector2  operator * (float scalar);
		Vector2  operator / (float scalar);
		bool	 operator== (const Vector2& value)const;
		bool	 operator!= (const Vector2& value)const;

		//	�x�N�g���̑傫�������߂�
		float Length()const;

		//	�x�N�g���̑傫���̓��l�����߂�
		float LengthSqr()const;

		//���K��
		Vector2& Normalize();

		//�Z�[�t�Ȑ��K��
		Vector2& SafeNormalize(const Vector2& value);

		//�e�����̐�Βl�̎擾
		static Vector2	Abs(const Vector2& value);
		static void		Abs(const Vector2& value, Vector2& result);

		//�^����ꂽ�͈͓��ɃN�����s���O
		static Vector2	Clamp(const Vector2& value, const Vector2& min, const Vector2& max);
		static void		Clamp(const Vector2& value, const Vector2& min, const Vector2& max, Vector2& result);

		//�l��0�`�P�ɐ�������
		static Vector2	Saturate(const Vector2& value);
		static void		Saturate(const Vector2& value, Vector2& result);

		//��̃x�N�g���Ԃ̋��������߂�
		static float	Distance(const Vector2& a, const Vector2& b);
		static void		Distance(const Vector2& a, const Vector2& b, float& result);

		//��̃x�N�g���Ԃ̂̋����̓��l�����߂�
		static float	DistanceSqr(const Vector2& a, const Vector2& b);
		static void		DistanceSqr(const Vector2& a, const Vector2& b, float& result);

		//����
		static float	Dot(const Vector2& a, const Vector2& b);
		static void		Dot(const Vector2& a, const Vector2& b, float& result);

		//�^����ꂽ�l�̐��K��
		static Vector2	Normalize(const Vector2& value);
		static void		Normalize(const Vector2& value, Vector2& result);

		//�^����ꂽ�l���琳�K�������݂�
		static Vector2	SafeNormalize(const Vector2& value, const Vector2& set);
		static void		SafeNormalize(const Vector2& value, const Vector2& set, Vector2& result);

		//�����p�����߂�
		static float	ComputeCrossingAngle(const Vector2& a, const Vector2& b);
		static void		ComputeCrossingAngle(const Vector2& a, const Vector2& b, float result);

		//�e�����̍ŏ��l�����߂܂�
		static Vector2	Min(const Vector2& a, const Vector2& b);
		static void		Min(const Vector2& a, const Vector2& b, Vector2& result);

		//�e�����̍ő�l�����߂܂�
		static Vector2	Max(const Vector2& a, const Vector2& b);
		static void		Max(const Vector2& a, const Vector2& b, Vector2& result);

		//���˃x�N�g���Ɩ@�����甽�˃x�N�g�������߂�
		static Vector2	Reflect(const Vector2& in, const Vector2& norm);
		static void		Reflect(const Vector2& in, const Vector2& norm, Vector2& result);

		//���˃x�N�g���Ɩ@���Ƌ��܊p������܃x�N�g�������߂�
		static Vector2	Reflect(const Vector2& in, const Vector2& norm, const float eta);
		static void		Reflect(const Vector2& in, const Vector2& norm, const float eta, Vector2& result);

		//�d�S���W�����߂�
		static Vector2	Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const float f, const float g);
		static void		Barycentric(const Vector2& a, const Vector2& b, const Vector2& c, const float f, const float g, Vector2& result);

		//�G���~�[�g�X�v���C����Ԃ��s��
		static Vector2	Hermite(const Vector2& a, const Vector2& t1, const Vector2& b, const Vector2& t2, const float amount);
		static void		Hermite(const Vector2& a, const Vector2& t1, const Vector2& b, const Vector2& t2, const float amount, Vector2& result);

		//Catmull-Rom��Ԃ��s��
		static Vector2  CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, const float amount);
		static void		CatmullRom(const Vector2& a, const Vector2& b, const Vector2& c, const Vector2& d, const float amount, Vector2& result);

		//���`��Ԃ��s��
		static Vector2	Lerp(const Vector2& a, const Vector2& b, const float amount);
		static void		Lerp(const Vector2& a, const Vector2& b, const float amount, Vector2& result);

		//�O�����������A��̒l��⊮����
		static Vector2	SmoothStep(const Vector2& a, const Vector2& b, const float amount);
		static void		SmoothStep(const Vector2& a, const Vector2& b, const float amount, Vector2 &result);

		//�s���p���ĕϊ�
		static Vector2	Transform(const Vector2& position, const Matrix& matrix);
		static void		Transform(const Vector2& position, const Matrix& matrix, Vector2 &result);

		//�s���p���Ė@����ϊ�
		static Vector2	TransformNormal(const Vector2& normal, const Matrix& matrix);
		static void		TransformNormal(const Vector2& normal, const Matrix& matrix, Vector2 &result);

		//�s���p���ăx�N�g����ϊ����C�ϊ����ʂ�w=1�Ɏˉe
		static Vector2	TransformCoord(const Vector2& coords, const Matrix& matrix);
		static void		TransformCoord(const Vector2& coords, const Matrix& matrix, Vector2 &result);

	private:

	};

	//==================================
	//Vector3
	//==================================
	struct  Vector3 {
		//==================================
		//�t�����h
		//==================================
		friend Vector3 operator* (float scalar, const Vector3& value);
		friend DirectX::XMFLOAT3 operator +(DirectX::XMFLOAT3& a, const Vector3& b);
		friend DirectX::XMFLOAT3 operator -(DirectX::XMFLOAT3& a, const Vector3& b);

	public:
		//==================================
		//�����o
		//==================================
		float x;
		float y;
		float z;

		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 forward;
		static const Vector3 down;
		static const Vector3 back;
		static const Vector3 left;
		static const Vector3 zero;
		static const Vector3 one;

	private:
		//==================================
		//���\�b�h
		//==================================
	public:
		Vector3();
		Vector3(const float x, const float y, const float z);
		Vector3(const Vector3& nVec);
		Vector3(Vector3&& other);
#ifdef INCLUDE_VECTOR_MATH_AOS
		Vector3(const Vectormath::Aos::Vector3& nVec) : 
			x(nVec.getX()), y(nVec.getY()), z(nVec.getZ())
		{
		
		};
		Vector3(Vectormath::Aos::Vector3&& other)
		{
			*this = std::move(other);
		};
#endif
		Vector3(const Vector4& nVec);
		Vector3(Vector4&& other);
		Vector3(const DirectX::XMFLOAT3& other);
		Vector3(DirectX::XMFLOAT3&& other);
		~Vector3();

		Vector3& operator = (const Vector3& value);
		Vector3& operator = (Vector3&& value);
		Vector3& operator = (const Vector4& value);
		Vector3& operator = (Vector4&& value);
		Vector3& operator = (const DirectX::XMFLOAT3& value);
		Vector3& operator = (DirectX::XMFLOAT3&& value);

#ifdef INCLUDE_VECTOR_MATH_AOS
		Vector3& operator = (const Vectormath::Aos::Vector3& value)
		{	
			this->x = value.getX();
			this->y = value.getY();
			this->z = value.getZ();
			return (*this);
		};
		Vector3& operator = (Vectormath::Aos::Vector3&& value)
		{
			this->x = value.getX();
			this->y = value.getY();
			this->z = value.getZ();
			value.setX(0.0f);
			value.setY(0.0f);
			value.setZ(0.0f);
			return (*this);
		};

		Vector3& operator = (const Vectormath::Aos::Vector4& value)
		{
			this->x = value.getX();
			this->y = value.getY();
			this->z = value.getZ();
			return (*this);
		};
		Vector3& operator = (Vectormath::Aos::Vector4&& value)
		{
			this->x = value.getX();
			this->y = value.getY();
			this->z = value.getZ();
			value.setX(0.0f);
			value.setY(0.0f);
			value.setZ(0.0f);
			value.setW(0.0f);
			return (*this);
		};
#endif
		Vector3& operator +=(const Vector3& value);
		Vector3& operator +=(const DirectX::XMFLOAT3& value);
		Vector3& operator -=(const Vector3& value);
		Vector3& operator -=(const DirectX::XMFLOAT3& value);
		Vector3& operator *=(float scalar);
		Vector3& operator /=(float scalar);
		Vector3  operator + ()const;
		Vector3	 operator - ()const;
		Vector3	 operator + (const Vector3& value)const;
		Vector3	 operator + (const DirectX::XMFLOAT3& value)const;
		Vector3	 operator - (const Vector3& value)const;
		Vector3	 operator - (const DirectX::XMFLOAT3& value)const;
		Vector3  operator * (float scalar);
		Vector3  operator / (float scalar);
		bool	 operator== (const Vector3& value)const;
		bool	 operator== (const DirectX::XMFLOAT3& value)const;
		bool	 operator!= (const Vector3& value)const;
		bool	 operator!= (const DirectX::XMFLOAT3& value)const;

		//�x�N�g���̑傫�����擾
		float Length()const;

		//�x�N�g���̑傫���̓��l���擾
		float LengthSqr()const;

		//���K��
		Vector3& Normalize();

		//���K�������݂�
		Vector3& SafeNormalize(const Vector3& value);

		//�e�����̐�Βl���擾
		static Vector3				Abs(const Vector3& value);
		static void					Abs(const Vector3& value, Vector3& result);

		//�^����ꂽ�l��͈͂��N�����v
		static Vector3				Clamp(const Vector3& value, const Vector3& min, const Vector3& max);
		static void					Clamp(const Vector3& value, const Vector3& min, const Vector3& max, Vector3& result);

		//�^����ꂽ��ԃx�N�g���𒷂��ŃN�����v
		static Vector3				ClampLength(const Vector3& value, float min, float max);
		static Vector3				ClampLength(const Vector3& value, float min, float max, Vector3& result);

		//�^����ꂽ�l��0�`1�Ő�������
		static Vector3				Saturate(const Vector3& value);
		static void					Saturate(const Vector3& value, Vector3& result);

		//��̃x�N�g���Ԃ̋��������߂�
		static float				Distance(const Vector3& a, const Vector3& b);
		static void					Distance(const Vector3& a, const Vector3& b, float& result);

		//��̃x�N�g���Ԃ̋����̓��l�����߂�
		static float				DistanceSqr(const Vector3& a, const Vector3& b);
		static void					DistanceSqr(const Vector3& a, const Vector3& b, float& result);

		//����
		static float				Dot(const Vector3& a, const Vector3& b);
		static void					Dot(const Vector3& a, const Vector3& b, float& result);

		//�O��
		static Vector3				Cross(const Vector3& a, const Vector3& b);
		static void					Cross(const Vector3& a, const Vector3& b, Vector3& result);

		//���K��
		static Vector3				Normalize(const Vector3& value);
		static void					Normalize(const Vector3& value, Vector3& result);

		//���K�������݂�
		static Vector3				SafeNormalize(const Vector3& value, const Vector3& set);
		static void					SafeNormalize(const Vector3& value, const Vector3& set, Vector3& result);

		//�@�������߂�
		static Vector3				ComputeNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3);
		static void					ComputeNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, Vector3& result);

		//�N�A�b�h�̖@�������߂�
		static Vector3				ComputeQuadNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4);
		static void					ComputeQuadNormal(const Vector3& p1, const Vector3& p2, const Vector3& p3, const Vector3& p4, Vector3& result);

		//�����p�����߂�
		static float				ComputeCrossingAngle(const Vector3& a, const Vector3& b);
		static void					ComputeCrossingAngle(const Vector3& a, const Vector3& b, float result);

		//�e�����̍ŏ��l�����߂�
		static Vector3				Min(const Vector3& a, const Vector3& b);
		static void					Min(const Vector3& a, const Vector3& b, Vector3& result);

		//�e�����̍ő�l�����߂�
		static Vector3				Max(const Vector3& a, const Vector3& b);
		static void					Max(const Vector3& a, const Vector3& b, Vector3& result);

		//���˃x�N�g���Ɩ@���Ŕ��˃x�N�g�������߂�
		static Vector3				Reflect(const Vector3& in, const Vector3& norm);
		static void					Reflect(const Vector3& in, const Vector3& norm, Vector3& result);

		//���˃x�N�g���Ɩ@���Ƌ��܊p������܃x�N�g�������߂�
		static Vector3				Reflect(const Vector3& in, const Vector3& norm, const float eta);
		static void					Reflect(const Vector3& in, const Vector3& norm, const float eta, Vector3& result);

		//�d�S�����߂�
		static Vector3				Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const float f, const float g);
		static void					Barycentric(const Vector3& a, const Vector3& b, const Vector3& c, const float f, const float g, Vector3& result);

		//�G���~�[�g�X�v���C����Ԃ������Ȃ�
		static Vector3				Hermite(const Vector3& a, const Vector3& t1, const Vector3& b, const Vector3& t2, const float amount);
		static void					Hermite(const Vector3& a, const Vector3& t1, const Vector3& b, const Vector3& t2, const float amount, Vector3& result);

		//Catmull-Rom�Ȑ���Ԃ��s��
		static Vector3				CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, const float amount);
		static void					CatmullRom(const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d, const float amount, Vector3& result);

		//���`��Ԃ��s��
		static Vector3				Lerp(const Vector3& a, const Vector3& b, const float amount);
		static void					Lerp(const Vector3& a, const Vector3& b, const float amount, Vector3& result);

		//��̒l��⊮����
		static Vector3				SmoothStep(const Vector3& a, const Vector3& b, const float amount);
		static void					SmoothStep(const Vector3& a, const Vector3& b, const float amount, Vector3 &result);

		//�s���p���ĕϊ�����
		static Vector3				Transform(const Vector3& position, const Matrix& matrix);
		static void					Transform(const Vector3& position, const Matrix& matrix, Vector3 &result);

		//�s���p���Ė@����ϊ�����
		static Vector3				TransformNormal(const Vector3& normal, const Matrix& matrix);
		static void					TransformNormal(const Vector3& normal, const Matrix& matrix, Vector3 &result);

		//�s���p���ăx�N�g����ϊ����C�ϊ����ʂ�w=1�Ɏˉe
		static Vector3				TransformCoord(const Vector3& coords, const Matrix& matrix);
		static void					TransformCoord(const Vector3& coords, const Matrix& matrix, Vector3 &result);

		//�X�J���[�O�d��
		static float				ScalarTriple(const Vector3& a, const Vector3& b, const Vector3& c);
		static void					ScalarTriple(const Vector3& a, const Vector3& b, const Vector3& c, float& result);

		//�x�N�g���O�d��
		static Vector3				VectorTriple(const Vector3& a, const Vector3& b, const Vector3& c);
		static void					VectorTriple(const Vector3& a, const Vector3& b, const Vector3& c, Vector3& result);

		//�l�����ŉ�]
		static Vector3				Rotate(const Vector3& value, const Quaternion& rotation);
		static void 				Rotate(const Vector3& value, const Quaternion& rotation, Vector3& result);

		//�l�����ŋt��]
		static Vector3				InverseRotate(const Vector3& value, const Quaternion& rotation);
		static void 				InverseRotate(const Vector3& value, const Quaternion& rotation, Vector3& result);

		//DirectXmath�Ƃ̌݊���
		static DirectX::XMFLOAT3	Store(const Vector3& value);
		static void					Store(const Vector3& value, DirectX::XMFLOAT3& result);

	private:

	};

	//==================================
	//Vector4
	//==================================
	struct  Vector4 {
		//==================================
		//�t�����h
		//==================================
		friend Vector4 operator* (float scalar, const Vector4& value);

	public:
		//==================================
		//�����o
		//==================================

		float x;
		float y;
		float z;
		float w;
	private:
		//==================================
		//���\�b�h
		//==================================
	public:
		Vector4();
		Vector4(const Vector4& value);
		Vector4(Vector4&& other);
		Vector4(const DirectX::XMFLOAT4& value);
		Vector4(DirectX::XMFLOAT4&& other);
		Vector4(const Quaternion& value);
		Vector4(Quaternion&& other);
		Vector4(const float x, const float y, const float z, float w);
		Vector4(const Vector2& value, const float z, float w);
		Vector4(const DirectX::XMFLOAT2& value, const float z, float w);
		Vector4(const Vector3& value, float w);
		Vector4(const DirectX::XMFLOAT3& value, float w);
		~Vector4();

		Vector4& operator = (const Vector4& value);
		Vector4& operator = (Vector4&& value);
		Vector4& operator = (const Vector3& value);
		Vector4& operator = (const DirectX::XMFLOAT4& value);
		Vector4& operator = (DirectX::XMFLOAT4&& value);
		Vector4& operator = (Quaternion&& value);
		Vector4& operator +=(const Vector4& value);
		Vector4& operator +=(DirectX::XMFLOAT4& value);
		Vector4& operator -=(const Vector4& value);
		Vector4& operator -=(DirectX::XMFLOAT4& value);
		Vector4& operator *=(float scalar);
		Vector4& operator /=(float scalar);
		Vector4  operator + ()const;
		Vector4	 operator - ()const;
		Vector4	 operator + (const Vector4& value)const;
		Vector4	 operator + (const DirectX::XMFLOAT4& value)const;
		Vector4	 operator - (const Vector4& value)const;
		Vector4	 operator - (const DirectX::XMFLOAT4& value)const;
		Vector4  operator * (float scalar);
		Vector4  operator / (float scalar);
		bool	 operator== (const Vector4& value)const;
		bool	 operator== (const DirectX::XMFLOAT4& value)const;
		bool	 operator!= (const Vector4& value)const;
		bool	 operator!= (const DirectX::XMFLOAT4& value)const;

		//�x�N�g���̑傫�����擾
		float Length()const;
		//�x�N�g���̑傫���̓��l���擾
		float LengthSqr()const;

		//���K��
		Vector4& Normalize();
		//���K�������݂�
		Vector4& SafeNormalize(const Vector4& set);

		//XYZ�����̎擾

		//�e�����̐�Βl�����߂�
		static Vector4	Abs(const Vector4& value);
		static void		Abs(const Vector4& value, Vector4& result);

		//�^����ꂽ�l��͈͂ŃN�����s���O����
		static Vector4	Clamp(const Vector4& value, const Vector4& min, const Vector4& max);
		static void		Clamp(const Vector4& value, const Vector4& min, const Vector4& max, Vector4& result);

		//�^����ꂽ�l��0�`1�Ő�������
		static Vector4	Saturate(const Vector4& value);
		static void		Saturate(const Vector4& value, Vector4& result);

		//��̃x�N�g���Ԃ̋��������߂�
		static float	Distance(const Vector4& a, const Vector4& b);
		static void		Distance(const Vector4& a, const Vector4& b, float& result);

		//��̃x�N�g���Ԃ̋����̓��l�����߂�
		static float	DistanceSqr(const Vector4& a, const Vector4& b);
		static void		DistanceSqr(const Vector4& a, const Vector4& b, float& result);

		//����
		static float	Dot(const Vector4& a, const Vector4& b);
		static void		Dot(const Vector4& a, const Vector4& b, float& result);

		//���K��
		static Vector4	Normalize(const Vector4& value);
		static void		Normalize(const Vector4& value, Vector4& result);

		//���K�������݂�
		static Vector4	SafeNormalize(const Vector4& value, const Vector4& set);
		static void		SafeNormalize(const Vector4& value, const Vector4& set, Vector4& result);

		//�����p�����߂�
		static float	ComputeCrossingAngle(const Vector4& a, const Vector4& b);
		static void		ComputeCrossingAngle(const Vector4& a, const Vector4& b, float result);

		//�e�����̍ŏ��l�����Ƃ߂�
		static Vector4	Min(const Vector4& a, const Vector4& b);
		static void		Min(const Vector4& a, const Vector4& b, Vector4& result);

		//�e�����̍ő�l�����Ƃ߂�@
		static Vector4	Max(const Vector4& a, const Vector4& b);
		static void		Max(const Vector4& a, const Vector4& b, Vector4& result);

		//�d�S�����߂�
		static Vector4	Barycentric(const Vector4& a, const Vector4& b, const Vector4& c, const float f, const float g);
		static void		Barycentric(const Vector4& a, const Vector4& b, const Vector4& c, const float f, const float g, Vector4& result);

		//�G���~�[�g�X�v���C����Ԃ��s��
		static Vector4	Hermite(const Vector4& a, const Vector4& t1, const Vector4& b, const Vector4& t2, const float amount);
		static void		Hermite(const Vector4& a, const Vector4& t1, const Vector4& b, const Vector4& t2, const float amount, Vector4& result);

		//Catmull-Rom�X�v���C����Ԃ��s��
		static Vector4  CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const float amount);
		static void		CatmullRom(const Vector4& a, const Vector4& b, const Vector4& c, const Vector4& d, const float amount, Vector4& result);

		//���`��Ԃ��s��
		static Vector4	Lerp(const Vector4& a, const Vector4& b, const float amount);
		static void		Lerp(const Vector4& a, const Vector4& b, const float amount, Vector4& result);

		//��̒l��⊮����
		static Vector4	SmoothStep(const Vector4& a, const Vector4& b, const float amount);
		static void		SmoothStep(const Vector4& a, const Vector4& b, const float amount, Vector4 &result);

		//�s���p���ĕϊ�����
		static Vector4	Transform(const Vector4& position, const Matrix& matrix);
		static void		Transform(const Vector4& position, const Matrix& matrix, Vector4 &result);
	private:

	};

	//==================================
	//Matrix
	//==================================
	struct  Matrix {
		//==============================
		//�t�����h
		//==============================
		friend Matrix operator * (float scalar, const Matrix& matrix);
		//==============================
		//�����o
		//==============================
	public:
		union {
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			struct
			{
				Vector4 axisX;
				Vector4 axisY;
				Vector4 axisZ;
				Vector4 axisW;
			};

			float matrix[4][4];
		};
	private:
		//==============================
		//���\�b�h
		//==============================


	public:
		Matrix();
		Matrix(const Matrix& mat);
		Matrix(Matrix&& mat);
		Matrix(const DirectX::XMMATRIX& mat);
		Matrix(DirectX::XMMATRIX&& mat);
		Matrix(const float m11, const float m12, const float m13, const float m14,
			const float m21, const float m22, const float m23, const float m24,
			const float m31, const float m32, const float m33, const float m34,
			const float m41, const float m42, const float m43, const float m44
		);
		Matrix(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4);
		Matrix(const DirectX::XMFLOAT4& v1, const DirectX::XMFLOAT4& v2, const DirectX::XMFLOAT4& v3, const DirectX::XMFLOAT4& v4);
		~Matrix();

		float& operator()(unsigned int row, unsigned int col);
		const float& operator()(unsigned int row, unsigned int col) const;

		Matrix& operator = (const Matrix& value);
		Matrix& operator = (Matrix&& value);
		Matrix& operator = (const DirectX::XMMATRIX& value);
		Matrix& operator = (DirectX::XMMATRIX&& value);
		Matrix& operator *=(const Matrix& value);
		Matrix& operator *=(const DirectX::XMMATRIX& value);
		Matrix& operator +=(const Matrix& value);
		Matrix& operator +=(const DirectX::XMMATRIX& value);
		Matrix& operator -=(const Matrix& value);
		Matrix& operator -=(const DirectX::XMMATRIX& value);
		Matrix& operator *=(float scalar);
		Matrix& operator /=(float scalar);
		Matrix  operator + ()const;
		Matrix	operator - ()const;
		Matrix	operator * (const Matrix& value)const;
		Matrix	operator * (const DirectX::XMMATRIX& value)const;
		Matrix	operator + (const Matrix& value)const;
		Matrix	operator + (const DirectX::XMMATRIX& value)const;
		Matrix	operator - (const Matrix& value)const;
		Matrix	operator - (const DirectX::XMMATRIX& value)const;
		Matrix  operator * (float scalar);
		Matrix  operator / (float scalar);
		bool	operator== (const Matrix& value)const;
		bool	operator== (const DirectX::XMMATRIX& value)const;
		bool	operator!= (const Matrix& value)const;
		bool	operator!= (const DirectX::XMMATRIX& value)const;

		// �s�񎮂����߂�
		float						Determinant()const;

		//�P�ʍs������߂�
		Matrix&						Identity();

		//�g�k�����𔲂��o��
		Vector3						Scale();

		//�ړ������𔲂��o��
		Vector3						Movement();

		//�P�ʍs����쐬
		static Matrix				CreateIdentity();

		//�s�񂪒P�ʍs��
		static bool					IsIdentity(const Matrix &value);
		bool						IsIdentity();

		//�]�u����
		static Matrix				Transpose(const Matrix& value);
		static void					Transpose(const Matrix& value, Matrix& result);
		Matrix&						Transpose();

		//�s�����Z����
		static Matrix				Multiply(const Matrix& a, const Matrix& b);
		static void					Multiply(const Matrix& a, const Matrix& b, Matrix& result);
		static Matrix				Multiply(const Matrix& value, const float scalar);
		static void					Multiply(const Matrix& value, const float scalar, Matrix& result);

		//�]�u��Z
		static Matrix				MultiplyTranspose(const Matrix& a, const Matrix& b);
		static void					MultiplyTranspose(const Matrix& a, const Matrix& b, Matrix& result);

		//�t�s������Ƃ߂�
		static Matrix				Invert(const Matrix& value);
		static void					Invert(const Matrix& value, Matrix& result);
		Matrix&						Invert();

		//�g�k�p�s��̍쐬
		static Matrix				CreateScaleMatrix(const float scale);
		static void					CreateScaleMatrix(const float scale, Matrix&);
		static Matrix				CreateScaleMatrix(const float sx, const float sy, const float sz);
		static void					CreateScaleMatrix(const float sx, const float sy, const float sz, Matrix& result);
		static Matrix				CreateScaleMatrix(const Vector3& scale);
		static void					CreateScaleMatrix(const Vector3& scale, Matrix& result);

		//�ړ��p�s��̍쐬
		static Matrix				CreateTranslationMatrix(const float tx, const float ty, const float tz);
		static void					CreateTranslationMatrix(const float tx, const float ty, const float tz, Matrix& result);
		static Matrix				CreateTranslationMatrix(const Vector3& scale);
		static void					CreateTranslationMatrix(const Vector3& scale, Matrix& result);

		//X������̉�]�s��̍쐬
		static Matrix				CreateRotationX(const float rad);
		static void					CreateRotationX(const float rad, Matrix& result);

		//Y������̉�]�s��̍쐬
		static Matrix				CreateRotationY(const float rad);
		static void					CreateRotationY(const float rad, Matrix& result);

		//Z������̉�]�s��̍쐬
		static Matrix				CreateRotationZ(const float rad);
		static void					CreateRotationZ(const float rad, Matrix& result);

		//�l��������s����쐬
		static Matrix				CreateFromQuaternion(const Quaternion& q);
		static void					CreateFromQuaternion(const Quaternion& q, Matrix& result);

		//���ƃ��W�A���p�����]�s����쐬
		static Matrix				CreateFromAxisAngles(const Vector3& axis, float rad);
		static void					CreateFromAxisAngles(const Vector3& axis, float rad, Matrix& result);

		//���[�s�b�`���[�������]�s������
		static Matrix				CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll);
		static void					CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, Matrix& result);

		//�r���[�s����쐬�i���b�N�A�b�g�s��j
		static Matrix				CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upward);
		static void					CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& upward, Matrix& result);

		//���b�N�A�b�g�s�������r���[�s��ł͂Ȃ�
		static Matrix				CreateLookAtObject(const Vector3& position, const Vector3& target, const Vector3& upward, const Vector3& rightward);
		static void					CreateLookAtObject(const Vector3& position, const Vector3& target, const Vector3& upward, const Vector3& rightward, Matrix& result);

		//�r���[�s����쐬(���b�N�g�D�s��)
		static Matrix				CreateLookTo(const Vector3& position, const Vector3& viewDir, const Vector3& upward);
		static void					CreateLookTo(const Vector3& position, const Vector3& viewDir, const Vector3& upward, Matrix& result);

		//�r���[�s�񂩂��]�s��𔲂��o��
		static Matrix				ExtractRotationMatrix(Matrix view);
		static void					ExtractRotationMatrix(Matrix view, Matrix& result);

		//�����ƕ�����ˉe�s����쐬
		static Matrix				CreatePerspective(const float width, const float height, const float nearClip, const float farClip);
		static void					CreatePerspective(const float width, const float height, const float nearClip, const float farClip, Matrix& result);

		//FOV����ˉe�s����쐬
		static Matrix				CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip);
		static void					CreatePerspectiveFOV(const float fieldOfView, const float aspectRatio, const float nearClip, const float farClip, Matrix& result);

		//�l���_����ˉe�s����쐬
		static Matrix				CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
		static void					CreatePerspectiveOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, Matrix& result);

		//���ˉe�s����쐬
		static Matrix				CreateOrthographic(const float width, const float height, const float nearClip, const float farClip);
		static void					CreateOrthographic(const float width, const float height, const float nearClip, const float farClip, Matrix& result);

		//���ˉe�s����쐬
		static Matrix				CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip);
		static void					CreateOrthographicOffcenter(const float left, const float right, const float top, const float bottom, const float nearClip, const float farClip, Matrix& result);

		//���`���
		static Matrix				Lerp(const Matrix& a, const Matrix& b, const float amount);
		static void					Lerp(const Matrix& a, const Matrix& b, const float amount, Matrix& result);

		void EpsilonCheck();

		//DirectXmath�Ƃ̌݊���
		static DirectX::XMMATRIX	Store(const Matrix& mat);
		static void					Store(const Matrix& mat, DirectX::XMMATRIX& result);

	private:
	};

	//===================================
	//Quaternion
	//===================================
	struct  Quaternion {
		//==================================
		//�����o
		//==================================
	public:
		float x;
		float y;
		float z;
		float w;
	private:
		//==================================
		//���\�b�h
		//==================================
	public:
		Quaternion();
		Quaternion(const Quaternion& other);
		Quaternion(Quaternion&& other);
		Quaternion(const Vector4& other);
		Quaternion(Vector4&& other);
		Quaternion(const float x, const float y, const float z, const float w);
		~Quaternion() {};

		Quaternion&			operator =  (const Quaternion& value);
		Quaternion&			operator =  (Quaternion&& value);
		Quaternion&			operator =  (const Vector4& value);
		Quaternion&			operator =  (Vector4&& value);
		Quaternion&			operator +=	(const Quaternion& value);
		Quaternion&			operator -=	(const Quaternion& value);
		Quaternion&			operator *=	(const Quaternion& value);
		Quaternion&			operator *=	(float scalar);
		Quaternion&			operator /=	(float scalar);
		Quaternion			operator +	()const;
		Quaternion			operator -	()const;
		Quaternion			operator *	(const Quaternion& value)const;
		Quaternion			operator +	(const Quaternion& value)const;
		Quaternion			operator -	(const Quaternion& value)const;
		Quaternion			operator *	(float scalar)const;
		Quaternion			operator /	(float scalar)const;
		bool				operator ==	(const Quaternion& value)const;
		bool				operator !=	(const Quaternion& value)const;

		//�傫�����擾
		float				Length()const;
		//�傫���̓��l���擾
		float				LengthSqr()const;
		//�I�C���[�p���擾
		Vector3				EulerAngles();

		//���K��
		Quaternion&			Normalize();
		//���K�������݂�
		Quaternion&			SafeNormalize(const Quaternion& set);

		//�P�ʎl���������߂�
		Quaternion&			Identity();

		//�P�ʎl�������쐬
		static Quaternion	CreateIdentity();

		//�P�ʎl�������ǂ���
		static bool			IsIdentity(const Quaternion& value);

		//�m�������P���ǂ���
		static bool			IsUnit(const Quaternion& value);

		//��Z
		static Quaternion	Multiply(const Quaternion& a, const Quaternion& b);
		static void			Multiply(const Quaternion& a, const Quaternion& b, Quaternion& reslut);

		//����
		static float		Dot(const Quaternion& a, const Quaternion& b);
		static void			Dot(const Quaternion& a, const Quaternion& b, float& reslut);

		//�����Ȏl���������߂�
		static Quaternion	Conjugate(const Quaternion& value);
		static void			Conjugate(const Quaternion& value, Quaternion& result);

		//���K��
		static Quaternion	Normalize(const Quaternion& value);
		static void			Normalize(const Quaternion& value, Quaternion& result);

		//���K�������݂�
		static Quaternion	SafeNormalize(const Quaternion& value, const Quaternion& set);
		static void			SafeNormalize(const Quaternion& value, const Quaternion& set, Quaternion& result);

		//���W�A���p����l�������쐬
		static Quaternion	CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll);
		static void			CreateFromYawPitchRoll(const float yaw, const float pitch, const float roll, Quaternion& result);

		//�I�C���[�p����l�������쐬
		static Quaternion	CreateFromEulerAngles(const float x, const float y, const float z);
		static Quaternion	CreateFromEulerAngles(const Vector3& eulerAngles);
		static void			CreateFromEulerAngles(const float x, const float y, const float z, Quaternion& result);
		static void			CreateFromEulerAngles(const Vector3& eulerAngles, Quaternion& result);

		//���ƃ��W�A���p����l�������쐬
		static Quaternion	CreateFromAxisAngles(const Vector3& axis, float radian);
		static void			CreateFromAxisAngles(const Vector3& axis, float radian, Quaternion& result);

		//��]�s�񂩂�l�������쐬
		static Quaternion	CreateFromRotationMatrix(const Matrix& value);
		static void			CreateFromRotationMatrix(const Matrix& value, Quaternion& result);

		//���ʐ��`���
		static Quaternion	Slerp(const Quaternion& a, const Quaternion& b, float amount);
		static void			Slerp(const Quaternion& a, const Quaternion& b, float amount, Quaternion& result);
		//���ʎl�p�`���
		static Quaternion	Squad(const Quaternion& value, const Quaternion& a, const Quaternion& b, const Quaternion& c, float amount);
		static void			Squad(const Quaternion& value, const Quaternion& a, const Quaternion& b, const Quaternion& c, float amount, Quaternion& result);

	private:
	};

	//===================================
	//OrthonormalBasis
	//===================================
	struct  OrthonormalBasis {
		//==================================
		//�����o
		//==================================
	public:
		Vector3 u;
		Vector3 v;
		Vector3 w;
	private:
		//==================================
		//���\�b�h
		//==================================
	public:
		OrthonormalBasis();
		OrthonormalBasis(const OrthonormalBasis& value);
		OrthonormalBasis(OrthonormalBasis&& value);
		OrthonormalBasis(const Vector3& nU, const Vector3& nV, const Vector3& nW);

		void InitFromU(const Vector3& nU);
		void InitFromV(const Vector3& nV);
		void InitFromW(const Vector3& nW);
		void InitFromUV(const Vector3& nU, const Vector3& nV);
		//void InitFromVU(const Vector3& nV, const Vector3& nU);
		void InitFromUW(const Vector3& nU, const Vector3& nW);
		//void InitFromWU(const Vector3& nW, const Vector3& nU);
		void InitFromVW(const Vector3& nV, const Vector3& nW);
		//void InitFromWV(const Vector3& nW, const Vector3& nV);

		OrthonormalBasis& operator = (const OrthonormalBasis& value);
		OrthonormalBasis& operator = (OrthonormalBasis&& value);

		bool operator == (const OrthonormalBasis& value)const;
		bool operator != (const OrthonormalBasis& value)const;

	};

	//�C�[�W���O�֐��Q
	namespace Easing {

		namespace Linear {
			float  In(float startValue, float changeValue, float time, float duration);
		};
		namespace Quadratic {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Cubic {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Quartic {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Quintic {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Shinusoidal {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Exponential {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
		namespace Circuler {
			float  In(float startValue, float changeValue, float time, float duration);
			float  Out(float startValue, float changeValue, float time, float duration);
			float  InOut(float startValue, float changeValue, float time, float duration);
		};
	};//Easing

}

//DirectXMathOperator
DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b);

DirectX::XMFLOAT3 operator + (const DirectX::XMFLOAT3& a);

DirectX::XMFLOAT3 operator - (const DirectX::XMFLOAT3& a);