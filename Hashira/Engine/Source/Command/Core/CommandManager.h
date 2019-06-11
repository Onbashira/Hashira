#pragma once
#include "Engine/Source/Utility/ManagerComponent.h"
#include "Engine/Source/Command/Core/InternalCommand.h"
#include <list>

namespace Hashira {

	struct InternalCommand;
	class CommandManager
	{
	public:

	private:

		//中間描画コマンドリスト
		std::list<InternalCommand> command;

	public:
		
		CommandManager() {};
		
		~CommandManager() {};
		
		void RecordCommand(InternalCommand& command) {};


	private:
	};
}