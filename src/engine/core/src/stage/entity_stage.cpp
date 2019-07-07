#include "stage/entity_stage.h"
#include <halley/entity/world.h>
#include <halley/entity/system.h>
#include "halley/file_formats/config_file.h"
#include "game/game.h"
using namespace Halley;

std::unique_ptr<World> EntityStage::createWorld(String configName,
												std::function<std::unique_ptr<System>(String)> createSystemFunction,
												std::function<void(Halley::EntityRef&, String, Halley::ConfigNode&)> createComponentFunction )
{
	auto world = std::make_unique<World>(&getAPI(), getGame().isDevMode());

	auto config = getResource<ConfigFile>(configName);
	world->loadSystems(config->getRoot(), createSystemFunction);
	world->loadEntities(config->getRoot(), createComponentFunction );

	return world;
}
