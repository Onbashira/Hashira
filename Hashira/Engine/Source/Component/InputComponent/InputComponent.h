#pragma once
namespace Hashira {

	class GameObject;

	class InputManager;

	//���[�U������I�u�W�F�N�g�̑�����T�|�[�g����x�[�X�R���|�l���g
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