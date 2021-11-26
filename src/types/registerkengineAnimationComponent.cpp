
#include "helpers/registerTypeHelper.hpp"
#include "data/AnimationComponent.hpp"

void registerkengineAnimationComponent() noexcept {
	kengine::registerComponents<kengine::AnimationComponent>();
}