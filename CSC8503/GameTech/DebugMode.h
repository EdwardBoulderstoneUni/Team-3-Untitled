#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <string.h>

#include <windows.h>  
#include <psapi.h>  
#include <direct.h>
#include <process.h>

#include <cstdint>
#define TIMER_START(_X)  auto _X##_start = std::chrono::steady_clock::now(), _X##_stop = _X##_start
#define TIMER_RESET(_X)  _X##_start = std::chrono::steady_clock::now(), _X##_stop = _X##_start
#define TIMER_STOP(_X)   _X##_stop = std::chrono::steady_clock::now()
#define TIMER_MSEC(_X)  (0.000001 *  std::chrono::duration_cast<std::chrono::nanoseconds>(_X##_stop - _X##_start).count())


inline int GetCurrentPid()
{
    return _getpid();
}

static uint64_t convert_time_format(const FILETIME* ftime)
{
    LARGE_INTEGER li;

    li.LowPart = ftime->dwLowDateTime;
    li.HighPart = ftime->dwHighDateTime;
    return li.QuadPart;
}

inline float GetCpuUsageRatio(int pid)
{
    static int64_t last_time = 0;
    static int64_t last_system_time = 0;

    FILETIME now;
    FILETIME creation_time;
    FILETIME exit_time;
    FILETIME kernel_time;
    FILETIME user_time;
    int64_t system_time;
    int64_t time;
    int64_t system_time_delta;
    int64_t time_delta;


    SYSTEM_INFO info;
    GetSystemInfo(&info);
    int cpu_num = info.dwNumberOfProcessors;

    float cpu_ratio = 0.0;

    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    GetSystemTimeAsFileTime(&now);

    if (!GetProcessTimes(process, &creation_time, &exit_time, &kernel_time, &user_time))
    {
        printf("GetCpuUsageRatio GetProcessTimes failed\n");
        return 0.0;
    }

    system_time = (convert_time_format(&kernel_time) + convert_time_format(&user_time)) / cpu_num;
    time = convert_time_format(&now);

    if ((last_system_time == 0) || (last_time == 0))
    {
        last_system_time = system_time;
        last_time = time;
        return 0.0;
    }

    system_time_delta = system_time - last_system_time;
    time_delta = time - last_time;

    CloseHandle(process);

    if (time_delta == 0)
    {
        printf("GetCpuUsageRatio time_delta is 0, error\n");
        return 0.0;
    }

    cpu_ratio = (int)((system_time_delta * 100 + time_delta / 2) / time_delta);
    last_system_time = system_time;
    last_time = time;
    cpu_ratio /= 100.0;

    return cpu_ratio;
}

inline float GetMemoryUsage(int pid)
{
    uint64_t mem = 0, vmem = 0;
    PROCESS_MEMORY_COUNTERS pmc;
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (GetProcessMemoryInfo(process, &pmc, sizeof(pmc)))
    {
        mem = pmc.PagefileUsage;
        vmem = pmc.PagefileUsage;
    }
    CloseHandle(process);
    return mem / 1024.0 / 1024.0;
}

void Memoryfootprint()
{
    int current_pid = GetCurrentPid();
    float cpu_usage_ratio = GetCpuUsageRatio(current_pid);
    float memory_usage = GetMemoryUsage(current_pid);

    Debug::Print("cpu usage:" + std::to_string(int(cpu_usage_ratio * 100)) + "%", Vector2(55, 70));
    Debug::Print("memory usage:" + std::to_string(int(memory_usage)) + "MB", Vector2(55, 75));
}