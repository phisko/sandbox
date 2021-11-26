
#include "helpers/registerTypeHelper.hpp"
#include "functions/Execute.hpp"

void registerkenginefunctionsExecute() noexcept {
	kengine::registerComponents<kengine::functions::Execute>();
}