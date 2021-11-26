
#include "helpers/registerTypeHelper.hpp"
#include "data/PathfindingComponent.hpp"

void registerkenginePathfindingComponent() noexcept {
	kengine::registerComponents<kengine::PathfindingComponent>();
}