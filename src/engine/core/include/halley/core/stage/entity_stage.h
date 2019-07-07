#pragma once
#include "stage.h"

namespace Halley
{
	class System;
	class EntityRef;

	class EntityStage : public Stage
	{
	public:
		std::unique_ptr<World> createWorld(	String configName,
											std::function<std::unique_ptr<System>(String)> createSystemFunction,
											std::function<void(Halley::EntityRef&, String, Halley::ConfigNode&)> createComponentFunction );
	};
}
