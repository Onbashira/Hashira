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
		//パスの実行前に呼ばれる関数
		virtual void BeforExecutionUpdate() = 0;
		//パスの実行後に呼ばれる関数
		virtual void AfterExecutionUpdate() = 0;
		//パスの実行（コマンド）
		virtual void ExecutePass() = 0;


	private:

	};

}