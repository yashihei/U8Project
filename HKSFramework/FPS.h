#pragma once
#pragma comment(lib, "winmm.lib")

#include <Windows.h>

class FPSControler {
public:
	FPSControler(int fps) : m_frameInterval(1000 / fps), m_fps(0.0), m_prevTime(0) {
		timeBeginPeriod(1);
		m_prevTime = timeGetTime();
	}
	~FPSControler() {
		timeEndPeriod(1);
	}
	void wait() {
		while (timeGetTime() - m_prevTime < m_frameInterval) {
			Sleep(1);
		}
		DWORD currentTime = timeGetTime();
		m_fps = 1000.0 / (currentTime - m_prevTime);
		m_prevTime = currentTime;
	}
	void changeFps(int fps) {
		m_frameInterval = 1000 / fps;
	}
	double getFps() const { return m_fps; }
private:
	unsigned int m_frameInterval;
	double m_fps;
	DWORD m_prevTime;
};
