#include "StreamOnHGlobal.h"

CStreamOnHGlobal::CStreamOnHGlobal(HGLOBAL hGlobal) : stream(nullptr) {
	if (FAILED(CreateStreamOnHGlobal(hGlobal, true, &stream))) {
		stream = nullptr;
	}
}

CStreamOnHGlobal::~CStreamOnHGlobal() {
	if (stream) {
		stream->Release();
		stream = nullptr;
	}
}

IStream* CStreamOnHGlobal::GetStream() const {
	return stream;
}

std::optional<std::vector<uint8_t>> CStreamOnHGlobal::ReadAll() {
	STATSTG stat = {};
	if (FAILED(stream->Stat(&stat, STATFLAG_NONAME))) {
		return std::nullopt;
	}

	if (FAILED(stream->Seek({}, STREAM_SEEK_SET, nullptr))) {
		return std::nullopt;
	}

	ULONG bytesRead = 0;
	std::vector<uint8_t> data(stat.cbSize.QuadPart);
	if (FAILED(stream->Read(data.data(), stat.cbSize.QuadPart, &bytesRead))) {
		return std::nullopt;
	}
	data.resize(bytesRead);

	return data;
}
