#pragma once
#include <iostream>
#include <chrono>
#include <ctime>

using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

class Time
{
private:
	Time()
	{
		deltaTime = 0.0f;
	}

	long long millisec_since_lastFrame = 0;

	float deltaTime = 0.0f; //in seconds

	void iUpdate();

public:
	Time(const Time&) = delete;

	static Time& Get()
	{
		static Time static_Instance;
		return static_Instance;
	}

	static void Update()
	{
		Get().iUpdate();
	}

	static float GetDeltaTime()
	{
		return Get().deltaTime;
	}

	static void Init()
	{
		Get().millisec_since_lastFrame = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	}
};

