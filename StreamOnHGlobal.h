#pragma once

#include <windows.h>
#include <cstdint>
#include <optional>
#include <vector>

class CStreamOnHGlobal {
protected:
	IStream* stream;

public:
	CStreamOnHGlobal(HGLOBAL hGlobal = nullptr);
	~CStreamOnHGlobal();

	IStream* GetStream() const;
	std::optional<std::vector<uint8_t>> ReadAll();
};
