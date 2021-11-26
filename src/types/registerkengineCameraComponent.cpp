
#include "helpers/registerTypeHelper.hpp"
#include "data/CameraComponent.hpp"

void registerkengineCameraComponent() noexcept {
	kengine::registerComponents<kengine::CameraComponent>();
}