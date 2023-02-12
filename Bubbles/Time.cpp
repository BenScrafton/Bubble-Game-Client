#include "Time.h"

void Time::iUpdate()
{
	deltaTime = 0.0f;

	long long millisec_since_epoch = duration_cast<std::chrono::milliseconds>(system_clock::now().time_since_epoch()).count();

	deltaTime = ((float)(millisec_since_epoch - millisec_since_lastFrame)) / 1000.0f;

	millisec_since_lastFrame = millisec_since_epoch;
}
