
#include "helpers/registerTypeHelper.hpp"
#include "data/SkyBoxComponent.hpp"

void registerkengineSkyBoxComponent() noexcept {
	kengine::registerComponents<kengine::SkyBoxComponent>();
}