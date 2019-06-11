#pragma once
namespace Hashira {
	class GameHeap;
	class CommandList;

	class RenderingPass
	{

	private:
		
	protected:

		std::shared_ptr<CommandList> _cmdList;

		std::shared_ptr<GameHeap> _gameHeap;

	public:

		RenderingPass(std::shared_ptr<CommandList> cmdList,std::shared_ptr<GameHeap>& _gameHeap);
		
		virtual ~RenderingPass();

		std::shared_ptr<CommandList> GetCommandList();
		//�p�X�̎��s�O�ɌĂ΂��֐�
		virtual void BeforExecutionUpdate() = 0;
		//�p�X�̎��s��ɌĂ΂��֐�
		virtual void AfterExecutionUpdate() = 0;
		//�p�X�̎��s�i�R�}���h�j
		virtual void ExecutePass() = 0;


	private:

	};

}