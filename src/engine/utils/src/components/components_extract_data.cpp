#include "halley/components/components_extract_data.h"
#include "halley/file_formats/config_file.h"

const Halley::ConfigNode& Halley::extractComponentData(const Halley::ConfigNode& data, size_t index, const char* name )
{
	if (data.getType() == Halley::ConfigNodeType::Map) {
		return data.asMap().at( name );
	} else {
		if (index < data.asSequence().size()) {
			return data.asSequence().at(index);
		} else {
			throw Exception(std::string("Tried to extract ") + (index + 1) + "th element from sequence with size " + data.asSequence().size(), HalleyExceptions::Entity);
		}
	}
}
