
#include "helpers/registerTypeHelper.hpp"
#include "data/LuaComponent.hpp"

void registerkengineLuaComponent() noexcept {
	kengine::registerComponents<kengine::LuaComponent>();
}