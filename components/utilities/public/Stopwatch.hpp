#pragma once


#include <chrono>
#include <stdint.h>


class Stopwatch
{
public:
	Stopwatch();

	void start();
	
	void stop();
	
	void reset();

	float averageTime() const;

	inline int recordCount() const;

private:
	using Clock = std::chrono::high_resolution_clock;

	Clock::duration _totalElapsedTime;
	Clock::time_point _lastStartTime;
	uint16_t _recordCount;
	bool _isRecording;

};


int Stopwatch::recordCount() const
{
	return _recordCount;
}
