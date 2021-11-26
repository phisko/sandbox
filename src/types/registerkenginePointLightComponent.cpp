
#include "helpers/registerTypeHelper.hpp"
#include "data/LightComponent.hpp"

void registerkenginePointLightComponent() noexcept {
	kengine::registerComponents<kengine::PointLightComponent>();
}