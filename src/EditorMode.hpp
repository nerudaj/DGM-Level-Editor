#pragma once

#include <string>

enum class EditorMode : std::size_t {
	Tiles,
	Items,
	Properties
};

namespace std {
	std::string to_string(EditorMode mode);
};