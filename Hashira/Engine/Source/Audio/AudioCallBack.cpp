#include "AudioCallBack.h"



Hashira::AudioCallBack::AudioCallBack() :
	 _onVoiceProcessingPassStart([](UINT32) {})
	, _onVoiceProcessingPassEnd([]() {}), _onStreamEnd([]() {}), _onBufferStart([](void*) {})
	, _onBufferEnd([](void*) {}), _onLoopEnd([](void*) {}), _onVoiceError([](void*,HRESULT) {})

{

}


Hashira::AudioCallBack::~AudioCallBack()
{
}


void Hashira::AudioCallBack::OnVoiceProcessingPassStart(UINT32 BytesRequired)
{
	_onVoiceProcessingPassStart(BytesRequired);
}

void Hashira::AudioCallBack::OnVoiceProcessingPassEnd(void)
{
	_onVoiceProcessingPassEnd();
}

void Hashira::AudioCallBack::OnStreamEnd(void)
{
	_onStreamEnd();
}

void Hashira::AudioCallBack::OnBufferStart(void * pBufferContext)
{
	_onBufferStart(pBufferContext);
}

void Hashira::AudioCallBack::OnBufferEnd(void * pBufferContext)
{
	_onBufferEnd(pBufferContext);
}

void Hashira::AudioCallBack::OnLoopEnd(void * pBufferContext)
{
	_onLoopEnd(pBufferContext);
}

void Hashira::AudioCallBack::OnVoiceError(void * pBufferContext, HRESULT Error)
{

	_onVoiceError(pBufferContext, Error);
}

void Hashira::AudioCallBack::SetOnVoiceProcessingPassStart(std::function<void(UINT32)> func)
{
	_onVoiceProcessingPassStart = func;
}

void Hashira::AudioCallBack::SetOnVoiceProcessingPassEnd(std::function<void()> func)
{
	_onVoiceProcessingPassEnd = func;
}

void Hashira::AudioCallBack::SetOnStreamEnd(std::function<void()> func)
{
	_onStreamEnd = func;
}

void Hashira::AudioCallBack::SetOnBufferStart(std::function<void(void*)> func)
{
	_onBufferStart = func;
}

void Hashira::AudioCallBack::SetOnBufferEnd(std::function<void(void*)> func)
{
	_onBufferEnd = func;
}

void Hashira::AudioCallBack::SetOnLoopEnd(std::function<void(void*)> func)
{
	_onLoopEnd = func;
}

void Hashira::AudioCallBack::SetOnVoiceError(std::function<void(void*, HRESULT)> func)
{
	_onVoiceError = func;
}
