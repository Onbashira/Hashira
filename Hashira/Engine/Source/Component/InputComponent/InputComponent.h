#pragma once
namespace Hashira {

	class GameObject;

	class InputManager;

	//ユーザ操作よりオブジェクトの操作をサポートするベースコンポネント
	class InputComponent
	{
	public:
	
	private:
	
	public:
		
		InputComponent() {};
		
		virtual ~InputComponent() {};
	
		virtual void Update(GameObject* gameObject , InputManager* inputManager) = 0;

	private:
	
	};
}