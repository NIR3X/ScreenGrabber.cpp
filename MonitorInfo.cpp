#include "MonitorInfo.h"

typedef enum PROCESS_DPI_AWARENESS {
	PROCESS_DPI_UNAWARE = 0,
	PROCESS_SYSTEM_DPI_AWARE = 1,
	PROCESS_PER_MONITOR_DPI_AWARE = 2
} PROCESS_DPI_AWARENESS;

STDAPI SetProcessDpiAwareness(PROCESS_DPI_AWARENESS value);

std::vector<SMonitorInfo> GetAllMonitorsInfo() {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE);

	std::vector<SMonitorInfo> monitors = {};

	EnumDisplayMonitors(nullptr, nullptr, [] (HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData) -> BOOL {
		((std::vector<SMonitorInfo>*)dwData)->push_back({
			.hMonitor = hMonitor,
			.hdcMonitor = hdcMonitor,
			.left = lprcMonitor->left,
			.top = lprcMonitor->top,
			.width = lprcMonitor->right - lprcMonitor->left,
			.height = lprcMonitor->bottom - lprcMonitor->top
		});
		return TRUE;
	}, (LPARAM)&monitors);

	return monitors;
}
