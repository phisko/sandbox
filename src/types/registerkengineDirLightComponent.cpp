
#include "helpers/registerTypeHelper.hpp"
#include "data/LightComponent.hpp"

void registerkengineDirLightComponent() noexcept {
	kengine::registerComponents<kengine::DirLightComponent>();
}