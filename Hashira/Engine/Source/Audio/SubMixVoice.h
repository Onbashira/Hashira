#pragma once
#include <xaudio2.h>
#include <memory>

namespace Hashira {
	class SubMixVoice
	{
		friend class SubMicVoiceManager;
	private:
		std::unique_ptr<IXAudio2SubmixVoice> _subMixVoice;
	public:
	private:
	public:
		SubMixVoice();
		~SubMixVoice();
	};
}

