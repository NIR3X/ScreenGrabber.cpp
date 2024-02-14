#pragma once

#include "MonitorInfo.h"
#include <windows.h>
#include <gdiplus.h>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

class CScreenGrabber {
public:
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	CLSID encoder;
	Gdiplus::EncoderParameters encoderParameters;

public:
	static std::optional<CLSID> GetEncoderClsid(const std::wstring& mimeType);

	CScreenGrabber();
	~CScreenGrabber();

	void SetEncoder(const CLSID& clsid);
	bool SetEncoder(const std::wstring& mimeType);
	void SetEncoderParameters(const Gdiplus::EncoderParameters& newEncoderParameters);

	std::optional<std::vector<uint8_t>> ScreenCapture(LONG left, LONG top, LONG width, LONG height);
	std::optional<std::vector<uint8_t>> ScreenCapture(const SMonitorInfo& monitorInfo);
};
