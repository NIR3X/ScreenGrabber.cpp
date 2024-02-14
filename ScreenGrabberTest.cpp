#include "MonitorInfo.h"
#include "ScreenGrabber.h"
#include <cassert>
#include <fstream>
#include <iostream>

int main() {
	CScreenGrabber screenGrabber = {};
	assert(screenGrabber.SetEncoder(L"image/jpeg"));

	ULONG quality = 15;
	screenGrabber.SetEncoderParameters({
		.Count = 1,
		.Parameter = {
			[0] = {
				.Guid = Gdiplus::EncoderQuality,
				.NumberOfValues = 1,
				.Type = Gdiplus::EncoderParameterValueTypeLong,
				.Value = &quality
			}
		}
	});

	std::vector<SMonitorInfo> allMonitors = GetAllMonitorsInfo();
	assert(allMonitors.size() > 0);
	for (size_t i = 0; i < allMonitors.size(); ++i) {
		auto& monitor = allMonitors[i];
		std::optional<std::vector<uint8_t>> result = screenGrabber.ScreenCapture(monitor);
		if (result.has_value()) {
			std::string filename = std::to_string(i) + ".jpg";
			std::ofstream outputFile(filename, std::ios::binary);
			if (outputFile.is_open()) {
				outputFile.write((const char*)result->data(), result->size());
				outputFile.close();
				std::cout << "Image captured from monitor " << i << " and saved as " << filename << std::endl;
			} else {
				std::cerr << "Failed to create file: " << filename << std::endl;
			}
		} else {
			std::cerr << "Failed to capture screen for monitor " << i << std::endl;
		}
	}

	return 0;
}
