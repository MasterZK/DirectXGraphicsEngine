#include "Time.h"
#include <Windows.h>
#include <string>

int Time::init()
{
    lastTimestamp = timeGetTime() / 1000.0f;

    return 0;
}

void Time::update()
{
    float actualTimestamp = timeGetTime() / 1000.0f;
    deltaTime = actualTimestamp - lastTimestamp;
    lastTimestamp = actualTimestamp;

    // CAUTION: only works with unicode setting
    std::wstring output = TEXT("deltaTime: ") + std::to_wstring(deltaTime) + TEXT("\n");
    OutputDebugString(output.c_str());
}

void Time::deInit()
{
}
