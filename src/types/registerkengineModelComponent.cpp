
#include "helpers/registerTypeHelper.hpp"
#include "data/ModelComponent.hpp"

void registerkengineModelComponent() noexcept {
	kengine::registerComponents<kengine::ModelComponent>();
}