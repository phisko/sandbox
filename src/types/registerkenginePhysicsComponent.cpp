
#include "helpers/registerTypeHelper.hpp"
#include "data/PhysicsComponent.hpp"

void registerkenginePhysicsComponent() noexcept {
	kengine::registerComponents<kengine::PhysicsComponent>();
}