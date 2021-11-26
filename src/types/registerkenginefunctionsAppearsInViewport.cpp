
#include "helpers/registerTypeHelper.hpp"
#include "functions/AppearsInViewport.hpp"

void registerkenginefunctionsAppearsInViewport() noexcept {
	kengine::registerComponents<kengine::functions::AppearsInViewport>();
}