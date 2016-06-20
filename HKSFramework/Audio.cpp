#include "Audio.h"

#include <stdexcept>
#include <vector>

AudioManager::AudioManager() :
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

AudioManager::~AudioManager()
{
	m_waveFiles.clear();
	for (auto& sourceVoice : m_sourceVoices)
		sourceVoice.second->DestroyVoice();
	if (m_masteringVoice)
		m_masteringVoice->DestroyVoice();
	if (m_xAudio)
		m_xAudio->Release();
	CoUninitialize();
}

void AudioManager::loadWave(std::string filePath, std::string alias) {
	auto waveFile = std::make_shared<WaveFile>(filePath);

	IXAudio2SourceVoice* sourceVoice;
	auto hr = m_xAudio->CreateSourceVoice(&sourceVoice, waveFile->getFormat());
	if (FAILED(hr))
		throw std::runtime_error("Error creating source voice");

	XAUDIO2_BUFFER buffer = {};
	buffer.pAudioData = waveFile->getData();
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.AudioBytes = waveFile->getSize();
	sourceVoice->SubmitSourceBuffer(&buffer);

	m_sourceVoices[alias] = sourceVoice;
	m_waveFiles.push_back(waveFile);
}

void AudioManager::play(std::string alias) {
	m_sourceVoices[alias]->Start();
}

void AudioManager::stop(std::string alias) {
	m_sourceVoices[alias]->Stop();
}

WaveFile::WaveFile(std::string filePath) {
	HMMIO hMmio = NULL;
	MMIOINFO mmioInfo = {};

	hMmio = mmioOpen(const_cast<LPSTR>(filePath.c_str()), &mmioInfo, MMIO_READ);
	if (!hMmio)
		throw std::runtime_error("Failed open" + filePath);
	
	MMRESULT mmResult;

	MMCKINFO riffChunk;
	riffChunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	mmResult = mmioDescend(hMmio, &riffChunk, NULL, MMIO_FINDRIFF);
	//TODO:error check

	MMCKINFO formatChunk;
	formatChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	mmResult = mmioDescend(hMmio, &formatChunk, &riffChunk, MMIO_FINDCHUNK);
	//TODO:error check

	mmioRead(hMmio, (HPSTR)&m_waveFormatEx, formatChunk.cksize);
	//TODO:error check

	mmioAscend(hMmio, &formatChunk, 0);

	MMCKINFO dataChunk;
	dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmResult = mmioDescend(hMmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);
	//TODO:error check

	m_data.resize(dataChunk.cksize);
	m_size = mmioRead(hMmio, (HPSTR)m_data.data(), dataChunk.cksize);
	//TODO:error check

	mmioClose(hMmio, 0);
}
