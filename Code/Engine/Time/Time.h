#pragma once

namespace cs6610
{
	namespace Time
	{
		void Initialize();
		double GetElapasedTimeTotal();
		double GetElapsedTimeDuringPreviousFrame();
		void OnNewFrame();
	}
}