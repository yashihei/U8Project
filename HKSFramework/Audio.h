#pragma once
#include <XAudio2.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class WaveFile;

class AudioManager {
public:
	AudioManager();
	~AudioManager();
	void loadWave(std::string filePath, std::string alias, bool loop = false);
	void play(std::string alias);
	void stop(std::string alias);
private:
	IXAudio2* m_xAudio;
	IXAudio2MasteringVoice* m_masteringVoice;

	std::unordered_map<std::string, IXAudio2SourceVoice*> m_sourceVoices;
	std::unordered_map<std::string, std::shared_ptr<WaveFile>> m_waveFiles;
	std::unordered_map<std::string, XAUDIO2_BUFFER> m_buffer;
};

class WaveFile {
public:
	explicit WaveFile(std::string filePath);
	DWORD getSize() const { return m_size; }
	const WAVEFORMATEX* getFormat() const { return &m_waveFormatEx; }
	BYTE* getData() { return m_data.data(); }
private:
	DWORD m_size;
	WAVEFORMATEX m_waveFormatEx;
	std::vector<BYTE> m_data;
};
