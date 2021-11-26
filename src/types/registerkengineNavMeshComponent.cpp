
#include "helpers/registerTypeHelper.hpp"
#include "data/NavMeshComponent.hpp"

void registerkengineNavMeshComponent() noexcept {
	kengine::registerComponents<kengine::NavMeshComponent>();
}