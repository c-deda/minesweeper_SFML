#include "timer.h"

Timer::Timer()
{
	Reset();
}

void Timer::Start()
{
	startTime = std::chrono::steady_clock::now();
}

void Timer::Reset()
{
	elapsedTime = std::chrono::duration<float>(0.0f);
}

void Timer::Update()
{
	elapsedTime = (std::chrono::steady_clock::now() - startTime);
}

int Timer::GetElapsedTime()
{
	if (elapsedTime.count() <= MAX_TIME)
	{
		return elapsedTime.count();
	}

	return MAX_TIME;
}