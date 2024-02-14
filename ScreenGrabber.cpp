#include "ScreenGrabber.h"
#include "StreamOnHGlobal.h"
#include <memory>

std::optional<CLSID> CScreenGrabber::GetEncoderClsid(const std::wstring& mimeType) {
	UINT num = 0, size = 0;
	Gdiplus::GetImageEncodersSize(&num, &size);
	if (size == 0) {
		return std::nullopt;
	}

	std::unique_ptr<uint8_t[]> encoders(new uint8_t[size]);
	Gdiplus::GetImageEncoders(num, size, (Gdiplus::ImageCodecInfo*)encoders.get());
	for (size_t i = 0; i < num; ++i) {
		auto& encoder = *((Gdiplus::ImageCodecInfo*)encoders.get() + i);
		if (encoder.MimeType == mimeType) {
			return encoder.Clsid;
		}
	}

	return std::nullopt;
}

CScreenGrabber::CScreenGrabber() : encoder({}), encoderParameters({}) {
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

CScreenGrabber::~CScreenGrabber() {
	Gdiplus::GdiplusShutdown(gdiplusToken);
}

void CScreenGrabber::SetEncoder(const CLSID& clsid) {
	encoder = clsid;
}

bool CScreenGrabber::SetEncoder(const std::wstring& mimeType) {
	std::optional<CLSID> clsid = GetEncoderClsid(mimeType);
	if (!clsid.has_value()) {
		return false;
	}

	SetEncoder(clsid.value());
	return true;
}

void CScreenGrabber::SetEncoderParameters(const Gdiplus::EncoderParameters& newEncoderParameters) {
	encoderParameters = newEncoderParameters;
}

std::optional<std::vector<uint8_t>> CScreenGrabber::ScreenCapture(LONG left, LONG top, LONG width, LONG height) {
	HDC hDc = GetDC(nullptr);
	if (!hDc) {
		return std::nullopt;
	}

	HDC hBitmapDC = nullptr;
	HBITMAP hBitmap = nullptr;
	HBITMAP hOldBitmap = nullptr;
	Gdiplus::Bitmap* bitmap = nullptr;
	CStreamOnHGlobal stream = {};
	std::optional<std::vector<uint8_t>> result = std::nullopt;

	hBitmapDC = CreateCompatibleDC(hDc);
	if (!hBitmapDC) {
		goto cleanup_dc;
	}

	hBitmap = CreateCompatibleBitmap(hDc, width, height);
	if (!hBitmap) {
		goto cleanup_bitmap_dc;
	}

	hOldBitmap = (HBITMAP)SelectObject(hBitmapDC, hBitmap);

	if (!BitBlt(hBitmapDC, 0, 0, width, height, hDc, left, top, SRCCOPY)) {
		goto cleanup_bitmap;
	}

	bitmap = Gdiplus::Bitmap::FromHBITMAP(hBitmap, nullptr);
	if (!bitmap) {
		goto cleanup_bitmap;
	}

	if (bitmap->Save(stream.GetStream(), &encoder, &encoderParameters) != Gdiplus::Ok) {
		goto cleanup_gdiplus;
	}

	result = stream.ReadAll();
	if (!result) {
		goto cleanup_gdiplus;
	}

cleanup_gdiplus:
	delete bitmap;

cleanup_bitmap:
	SelectObject(hBitmapDC, hOldBitmap);
	DeleteObject(hBitmap);

cleanup_bitmap_dc:
	DeleteDC(hBitmapDC);

cleanup_dc:
	ReleaseDC(nullptr, hDc);
	return result;
}

std::optional<std::vector<uint8_t>> CScreenGrabber::ScreenCapture(const SMonitorInfo& monitorInfo) {
	return ScreenCapture(monitorInfo.left, monitorInfo.top, monitorInfo.width, monitorInfo.height);
}
