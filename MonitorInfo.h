#pragma once

#include <windows.h>
#include <vector>

struct SMonitorInfo {
	HMONITOR hMonitor;
	HDC hdcMonitor;
	LONG left;
	LONG top;
	LONG width;
	LONG height;
};

std::vector<SMonitorInfo> GetAllMonitorsInfo();
