#include "Stopwatch.hpp"
#include <assert.h>


Stopwatch::Stopwatch() :
	_totalElapsedTime(),
	_lastStartTime(),
	_recordCount(0),
	_isRecording(false)
{

}


void Stopwatch::start()
{
	// Stopwatch must not be recording in order to call "Start"
	assert(!_isRecording);

	_lastStartTime = Clock::now();
	_isRecording = true;
}


void Stopwatch::stop()
{
	// Stopwatch must be recording in order to call "End"
	assert(_isRecording);
	
	
	auto currentTime = Clock::now();
	_totalElapsedTime += currentTime - _lastStartTime;
	_recordCount++;
	_isRecording = false;
}


void Stopwatch::reset()
{
	_isRecording = false;
	_totalElapsedTime = Clock::duration();
	_recordCount = 0;
}


float Stopwatch::averageTime() const
{
	int milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(_totalElapsedTime).count();
	return milliseconds / ((float)_recordCount * 1000.0f);
}