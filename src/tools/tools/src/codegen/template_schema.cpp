#include "../yaml/halley-yamlcpp.h"
#include <halley/tools/codegen/template_schema.h>
#include "yaml-cpp/yaml.h"

using namespace Halley;

TemplateSchema::TemplateSchema() {}

TemplateSchema::TemplateSchema(YAML::Node node)
{
	name = node["name"].as<std::string>();

	for (auto componentEntry : node["components"]) {
		String componentName = componentEntry.first.as<std::string>();

		if (components.find(componentName) == components.end()) {
			components.emplace(std::make_pair(componentName, ComponentMembers()));
		}
		ComponentMembers& members = components.find(componentName)->second;

		for (auto memberEntry : componentEntry.second) {
			for (auto m = memberEntry.begin(); m != memberEntry.end(); ++m) {
				members.emplace( m->first.as<std::string>(), m->second );
			}
		}
	}
}

TemplateSchema::~TemplateSchema() {}