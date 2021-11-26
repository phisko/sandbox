
#include "helpers/registerTypeHelper.hpp"
#include "data/PythonComponent.hpp"

void registerkenginePythonComponent() noexcept {
	kengine::registerComponents<kengine::PythonComponent>();
}