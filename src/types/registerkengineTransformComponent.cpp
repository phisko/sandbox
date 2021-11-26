
#include "helpers/registerTypeHelper.hpp"
#include "data/TransformComponent.hpp"

void registerkengineTransformComponent() noexcept {
	kengine::registerComponents<kengine::TransformComponent>();
}