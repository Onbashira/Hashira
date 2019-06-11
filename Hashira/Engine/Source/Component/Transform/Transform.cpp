#include "Transform.h"
#include <utility>

Hashira::Transform::Transform() :
	_pos(), _rotation(Quaternion::CreateIdentity()),_scale(Vector3::one)
{

}


Hashira::Transform::~Transform()
{
	_pos = Vector3::zero;
	_rotation = Quaternion::CreateIdentity();
}


void Hashira::Transform::SetPos(const Vector3  pos)
{
	_pos = pos;
}

void Hashira::Transform::SetRotation(const Quaternion rotation)
{
	_rotation = rotation;
	_eulerAngles = _rotation.EulerAngles();
}

void Hashira::Transform::SetEulerAngles(const Vector3 eulerAugles)
{
	_eulerAngles = eulerAugles;
	_rotation = std::move(Quaternion::CreateFromEulerAngles(eulerAugles));
}

void Hashira::Transform::SetScale(const Vector3 scale)
{
	_scale = scale;
}

Hashira::Vector3 Hashira::Transform::GetPos()
{
	return _pos;
}

Hashira::Quaternion  Hashira::Transform::GetRotation()
{
	return	_rotation;
}

Hashira::Vector3 Hashira::Transform::GetEulerAngles()
{
	return _rotation.EulerAngles();
}

Hashira::Vector3 Hashira::Transform::GetScale()
{
	return _scale;
}

void Hashira::Transform::Translate(const Vector3 & velocity)
{
	_pos += velocity;
}

void Hashira::Transform::Move(const Vector3 & velocity)
{
	_pos += Vector3::Rotate(velocity, _rotation);
}

void Hashira::Transform::Rotation(const Quaternion & rot)
{
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void Hashira::Transform::RotationLocalWorld(const Quaternion & rot)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();
}

void Hashira::Transform::RotationLocalWorld(const Vector3 & eulerAngles)
{
	//ローカル軸で回転させたい
	auto axis = GetLocalAxis();
	auto uRot = std::move(Quaternion::CreateFromAxisAngles(axis.u, DegToRad(eulerAngles.x)));
	auto vRot = std::move(Quaternion::CreateFromAxisAngles(axis.v, DegToRad(eulerAngles.y)));
	auto wRot = std::move(Quaternion::CreateFromAxisAngles(axis.w, DegToRad(eulerAngles.z)));

}

void Hashira::Transform::RotationEulerAngles(const Vector3 & eulerAngles)
{
	_eulerAngles += eulerAngles; 
	_rotation = std::move(Quaternion::CreateFromEulerAngles(_eulerAngles));
}

void Hashira::Transform::RotationAxisAngles(const Vector3 & axis, float rad)
{
	_rotation *= std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
}

void Hashira::Transform::RotateAround(const Vector3 & point, const Quaternion & rot)
{
	auto v = std::move(_pos - point);
	auto vv = std::move(Vector3::Rotate(v, rot));
	_rotation *= rot;
	_rotation.Normalize();
	_eulerAngles = _rotation.EulerAngles();
	_pos = std::move(vv + (point));
}

void Hashira::Transform::RotateAround(const Vector3 & point, const Vector3 & axis, float rad)
{
	auto q = std::move(Quaternion::CreateFromAxisAngles(axis, rad).Normalize());
	RotateAround(point, q);
}


void Hashira::Transform::LookAt(const Vector3 & target, const Vector3 & up)
{
	//ターゲットをローカル空間に持っていく
	Vector3 localTargetPos = target;
	localTargetPos = Vector3::Transform(localTargetPos, GetView());
	_rotation = std::move(Quaternion::CreateFromRotationMatrix(Matrix::ExtractRotationMatrix((Matrix::CreateLookAt(Vector3::zero, localTargetPos, up)))).Normalize());
	_eulerAngles = _rotation.EulerAngles();
}

Hashira::OrthonormalBasis Hashira::Transform::GetLocalAxis()
{
	OrthonormalBasis orthonomal;
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));
	orthonomal.u = Vector3(mat.axisX).Normalize();
	orthonomal.v = Vector3(mat.axisY).Normalize();
	orthonomal.w = Vector3(mat.axisZ).Normalize();
	return orthonomal;
}

Hashira::Matrix Hashira::Transform::GetView()
{
	//回転行列
	auto mat = Matrix::CreateFromQuaternion(_rotation);

	mat._41 = _pos.x;
	mat._42 = _pos.y;
	mat._43 = _pos.z;
	mat._44 = 1.0f;

	mat = Matrix::Invert(mat);
	//mat.EpsilonCheck();

	return mat;
}

Hashira::Matrix Hashira::Transform::GetSRTMatrix()
{
	auto mat = std::move(Matrix::CreateFromQuaternion(_rotation));

	mat = std::move(Matrix::CreateScaleMatrix(_scale) * mat);
	mat.axisW.x = _pos.x;
	mat.axisW.y = _pos.y;
	mat.axisW.z = _pos.z;
	mat.axisW.w = 1.0f;
	return mat;
}

