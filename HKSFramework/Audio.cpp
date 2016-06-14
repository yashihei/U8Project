#include "Audio.h"

#include <stdexcept>
#include <vector>
#include "SDKwavefile.h"

Audio::Audio() :
m_xAudio(NULL), m_masteringVoice(NULL)
{
	CoInitializeEx(NULL, COINIT_MULTITHREADED);
	
	UINT32 flags = 0;
#ifdef _DEBUG
	flags |= XAUDIO2_DEBUG_ENGINE;
#endif

	auto hr = XAudio2Create(&m_xAudio, flags);
	if (FAILED(hr))
		throw std::runtime_error("Error initialize XAudio2");

	hr = m_xAudio->CreateMasteringVoice(&m_masteringVoice);
	if (FAILED(hr))
		throw std::runtime_error("Error creating mastering voice");
}

Audio::~Audio()
{
	if (m_masteringVoice)
		m_masteringVoice->DestroyVoice();
	if (m_xAudio)
		m_xAudio->Release();
	CoUninitialize();
}

void Audio::loadWave(std::string fileName, std::string alias) {
	//waveファイルを開く
	CWaveFile wav;
	auto hr = wav.Open(const_cast<LPSTR>(fileName.c_str()), NULL, WAVEFILE_READ);
	if (FAILED(hr))
		throw std::runtime_error("Failed load" + fileName);

	//バッファにデータを書き込む
	DWORD waveSize = wav.GetSize();
	std::vector<BYTE> waveData(waveSize);
	hr = wav.Read(waveData.data(), waveSize, &waveSize);
	if (FAILED(hr))
		throw std::runtime_error("Failed read" + fileName);

	IXAudio2SourceVoice* sourceVoice;
	hr = m_xAudio->CreateSourceVoice(&sourceVoice, wav.GetFormat());
	if (FAILED(hr))
		throw std::runtime_error("Error creating source voice");

	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = waveData.data();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = waveSize;
	sourceVoice->SubmitSourceBuffer(&buffer);

	m_sourceVoices[alias] = sourceVoice;
}

void Audio::play(std::string alias) {
	m_sourceVoices[alias]->Start();
}

void Audio::stop(std::string alias) {
	m_sourceVoices[alias]->Stop();
}
