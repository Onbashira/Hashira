#include "AudioWaveSource.h"

Hashira::AudioWaveSource::AudioWaveSource() :
	_loadingInfo({}), _format({})
{
}

Hashira::AudioWaveSource::~AudioWaveSource()
{
	this->_wave.clear();
	this->_wave.shrink_to_fit();
}


WAVEFORMATEX & Hashira::AudioWaveSource::GetWaveFormat()
{
	return _format.Format;
}

WAVEFORMATEXTENSIBLE & Hashira::AudioWaveSource::GetWaveFormatExtensible()
{
	return _format;
}

std::vector<float>& Hashira::AudioWaveSource::GetWave()
{
	return _wave;
}

bool Hashira::AudioWaveSource::IsWaveLoaded()
{
	return _loadingInfo.isWaveLoaded;
}
