#include "InternalCommandCreater.h"

Hashira::InternalCommandCreater::InternalCommandCreater()
{
}

Hashira::InternalCommandCreater::~InternalCommandCreater()
{

}

void Hashira::InternalCommandCreater::SetThreadID(std::thread::id threadID)
{
	_threadID = threadID;
}

std::thread::id & Hashira::InternalCommandCreater::GetThreadID()
{	
	return this->_threadID;
}

void Hashira::InternalCommandCreater::SetThisThreadID()
{
	this->_threadID = std::this_thread::get_id();
}
