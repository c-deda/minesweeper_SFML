#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include "Definitions.h"

class Timer
{
public:
	Timer();

	void Start();
	void Reset();
	void Update();

	int GetElapsedTime();

private:
	std::chrono::steady_clock::time_point startTime;
	std::chrono::duration<float> elapsedTime;
};

#endif