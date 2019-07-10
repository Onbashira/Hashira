#pragma once
#include "Engine/Source/Resource/ConstantBuffer.h"
#include "Engine/Source/Component/Transform/Transform.h"

#include <memory>


namespace Hashira {

	class CommandList;
	class GraphicsComponent;
	class InputComponent;
	class PhysicsComponent;

	class GameObject
	{	

	public:

	protected:

		bool _isEnable;

		Transform _transform;

		ConstantBuffer _transformBuffer;

		std::unique_ptr<GraphicsComponent> _graphicsComponent;

		std::unique_ptr<InputComponent> _inputComponent;

		std::unique_ptr<PhysicsComponent> _physicsComponent;


	private:

	public:

		GameObject(GraphicsComponent* graphicsComponent,
			InputComponent* inputComponent,
			PhysicsComponent* physicsComponent
		);

		virtual ~GameObject();

		virtual void Update() = 0;

		virtual void Draw(std::shared_ptr<CommandList>& cmdList) = 0;

		void Enable();

		void Disable();

		bool IsEnable();

		Transform& GetTransform();

		virtual std::unique_ptr<GraphicsComponent>& GetGraphicsComponent();

		virtual std::unique_ptr<InputComponent>& GetInputComponent();

		virtual std::unique_ptr<PhysicsComponent>& GetPhysicsComponent();

	protected:

		virtual void UpdateTransformBuffer();
		//バッファリソースにSize分の1次元バッファとして領域を確保する；
		virtual void InitalizeTransformBuffer(size_t size);

	private:

	};
}
