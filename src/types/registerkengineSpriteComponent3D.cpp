
#include "helpers/registerTypeHelper.hpp"
#include "data/SpriteComponent.hpp"

void registerkengineSpriteComponent3D() noexcept {
	kengine::registerComponents<kengine::SpriteComponent3D>();
}