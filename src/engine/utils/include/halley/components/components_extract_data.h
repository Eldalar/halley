#pragma once

namespace Halley {
	class ConfigNode;

	const Halley::ConfigNode& extractComponentData(const Halley::ConfigNode& data, size_t index, const char* name );
}