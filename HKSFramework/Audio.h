#pragma once
#include <XAudio2.h>
#include <string>
#include <unordered_map>

class Audio {
public:
	Audio();
	~Audio();
	void loadWave(std::string filePath, std::string alias);
	void play(std::string alias);
	void stop(std::string alias);
	IXAudio2MasteringVoice* getMasteringVoice() { return m_masteringVoice; }
private:
	IXAudio2* m_xAudio;
	IXAudio2MasteringVoice* m_masteringVoice;
	std::unordered_map<std::string, IXAudio2SourceVoice*> m_sourceVoices;
};
