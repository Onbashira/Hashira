#include "GameObject.h"
#include "Engine/Source/CommandList/CommandList.h"
#include "Engine/Source/Component/GraphicsComponent/GraphicsComponent.h"
#include "Engine/Source/Component/InputComponent/InputComponent.h"
#include "Engine/Source/Component/PhysicsComponent/PhysicsComponent.h"

Hashira::GameObject::GameObject(Hashira::GraphicsComponent* graphicsComponent, 
	Hashira::InputComponent* inputComponent,
	Hashira::PhysicsComponent* physicsComponent):
	_transform({}), 
	_isEnable(true),
	_graphicsComponent(graphicsComponent),
	_inputComponent(inputComponent),
	_physicsComponent(physicsComponent)
{

}

Hashira::GameObject::~GameObject()
{
	_transformBuffer.Discard();
	
}


void Hashira::GameObject::Enable()
{
	_isEnable = true;
}

void Hashira::GameObject::Disable()
{
	_isEnable = false;
}

bool Hashira::GameObject::IsEnable()
{
	return _isEnable;
}

Hashira::Transform & Hashira::GameObject::GetTransform()
{
	return _transform;
}

std::unique_ptr<Hashira::GraphicsComponent>& Hashira::GameObject::GetGraphicsComponent()
{
	return _graphicsComponent;
}

std::unique_ptr<Hashira::InputComponent>& Hashira::GameObject::GetInputComponent()
{
	return _inputComponent;
}

std::unique_ptr<Hashira::PhysicsComponent>& Hashira::GameObject::GetPhysicsComponent()
{
	return _physicsComponent;
}

void Hashira::GameObject::UpdateTransformBuffer()
{
	auto mat = Matrix::CreateFromQuaternion(this->_transform.GetRotation());
	mat.axisW.x = _transform.GetPos().x;
	mat.axisW.y = _transform.GetPos().y;
	mat.axisW.z = _transform.GetPos().z;
	mat.axisW.w = 1.0f;

	mat = _transform.GetSRTMatrix();

	_transformBuffer.Update(&mat, _transformBuffer.GetInitializeSize(), 0);
}

void Hashira::GameObject::InitalizeTransformBuffer(size_t size)
{
	_transformBuffer.Initialize(size);
}