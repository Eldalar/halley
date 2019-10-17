#pragma once

#include "fields_schema.h"
#include "halley/data_structures/hash_map.h"
#include <unordered_set>

namespace YAML
{
	class Node;
}

namespace Halley
{
	class TemplateSchema
	{
	public:
		TemplateSchema();
		explicit TemplateSchema(YAML::Node node);
		~TemplateSchema();

		String name;
		using ComponentMembers = Halley::HashMap<String, YAML::Node>;
		using Components = Halley::HashMap<String, ComponentMembers>;
		Components components;
		std::unordered_set<String> includeFiles;
	};
}