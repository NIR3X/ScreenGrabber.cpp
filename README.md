# ScreenGrabber.cpp - Library for Capturing Screens on Windows Systems

The **ScreenGrabber** library provides functionality to capture screens or specific regions of the screen on Windows systems and save them as images. It supports various image formats, including BMP, GIF, JPEG, PNG, and TIFF, through the GDI+ library.

## Features

* Capture the entire screen or specific regions.
* Supports capturing multiple monitors.
* Save captured screens in BMP, GIF, JPEG, PNG, or TIFF formats with customizable options.
* Easy-to-use interface.

## Installation

To use the **ScreenGrabber** library in your project, follow these steps:

1. Clone this repository and compile it using a C++ compiler:

```bash
git clone https://github.com/NIR3X/ScreenGrabber.cpp
cd ScreenGrabber.cpp
make
```

2. Include the necessary header file `ScreenGrabber.h` in your project.

3. Link against the `ScreenGrabber.a` static library.

4. Ensure that your project is linked against the following libraries:

* `gdi32`
* `gdiplus`
* `ole32`
* `shcore`

## Usage

Here's a simple example demonstrating how to capture screens using the **ScreenGrabber** library:

```cpp
#include "ScreenGrabber.h"
#include <fstream>
#include <iostream>

int main() {
	// Create an instance of CScreenGrabber
	CScreenGrabber screenGrabber = {};

	// Set the encoder to the desired format (e.g., JPEG)
	std::wstring mimeType = L"image/jpeg";
	screenGrabber.SetEncoder(mimeType);

	// Set JPEG quality
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

	// Get information about all monitors
	std::vector<SMonitorInfo> allMonitors = GetAllMonitorsInfo();

	// Capture screens for each monitor
	for (size_t i = 0; i < allMonitors.size(); ++i) {
		auto& monitor = allMonitors[i];
		std::optional<std::vector<uint8_t>> result = screenGrabber.ScreenCapture(monitor);
		if (result.has_value()) {
			// Save captured image to file
			std::string filename = std::to_string(i) + ".jpg"; // Change extension based on encoder
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
```

## License
[![GNU AGPLv3 Image](https://www.gnu.org/graphics/agplv3-155x51.png)](https://www.gnu.org/licenses/agpl-3.0.html)  

This program is Free Software: You can use, study share and improve it at your
will. Specifically you can redistribute and/or modify it under the terms of the
[GNU Affero General Public License](https://www.gnu.org/licenses/agpl-3.0.html) as
published by the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.
