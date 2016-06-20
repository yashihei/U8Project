#pragma once
#include <XAudio2.h>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

class WaveFile;

class Audio {
public:
	Audio();
	~Audio();
	void loadWave(std::string filePath, std::string alias);
	void play(std::string alias);
	void stop(std::string alias);
private:
	IXAudio2* m_xAudio;
	IXAudio2MasteringVoice* m_masteringVoice;
	std::unordered_map<std::string, IXAudio2SourceVoice*> m_sourceVoices;
	std::vector<std::shared_ptr<WaveFile>> m_waveFiles;
};

class WaveFile {
public:
	WaveFile(std::string filePath);
	DWORD getSize() const { return m_size; }
	const WAVEFORMATEX* getFormat() const { return &m_waveFormatEx; }
	BYTE* getData() { return m_data.data(); }
private:
	DWORD m_size;
	WAVEFORMATEX m_waveFormatEx;
	std::vector<BYTE> m_data;
};
