#pragma once
#include "InternalCommand.h"
#include <list>
namespace Hashira {

	struct InternalCommand;

	class InternalCommandAllocator;

	struct NativeCommand;

	//中間命令を作る機能を持つ
	class InternalCommandCreater
	{

	public:

	private:

		//自身が生成されたthreadのID
		std::thread::id _threadID;

		//ミューテックス
		std::mutex _mutex;

	public:
	
		InternalCommandCreater();
	
		~InternalCommandCreater();
		 
		void SetThreadID(std::thread::id threadID);

		std::thread::id& GetThreadID();

		void SetThisThreadID();

		NativeCommand ConvertCommand();

	private:

	};

}