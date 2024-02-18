#include "MonitorInfo.h"
#include <thread>

std::vector<SMonitorInfo> GetAllMonitorsInfo() {
	std::vector<SMonitorInfo> monitors = {};

	std::thread([&monitors] {
		static HMODULE hUser32 = LoadLibraryA("user32.dll");
		static auto pSetThreadDpiAwarenessContext = (void*(WINAPI*)(void*))GetProcAddress(hUser32, "SetThreadDpiAwarenessContext");
		pSetThreadDpiAwarenessContext((void*)-3/* DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE */);

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
	}).join();

	return monitors;
}
