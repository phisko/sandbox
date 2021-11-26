
#include "helpers/registerTypeHelper.hpp"
#include "data/LuaTableComponent.hpp"

void registerkengineLuaTableComponent() noexcept {
	kengine::registerComponents<kengine::LuaTableComponent>();
}