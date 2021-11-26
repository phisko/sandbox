
#include "helpers/registerTypeHelper.hpp"
#include "data/InstanceComponent.hpp"

void registerkengineInstanceComponent() noexcept {
	kengine::registerComponents<kengine::InstanceComponent>();
}