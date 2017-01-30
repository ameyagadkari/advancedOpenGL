#include "Time.h"
#include "../../External/cyCodeBase/cyTimer.h"

namespace
{
	cyTimer s_timer;
	double s_currentTime = 0.0, s_previousTime = 0.0;
}

void cs6610::Time::Initialize()
{
	s_timer.Start();
}

double cs6610::Time::GetElapasedTimeTotal()
{
	return s_timer.Stop();
}

double cs6610::Time::GetElapsedTimeDuringPreviousFrame()
{
	return s_timer.Stop() - s_previousTime;
}

void cs6610::Time::OnNewFrame()
{
	s_previousTime = s_currentTime;
	s_currentTime = s_timer.Stop();
}