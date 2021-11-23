#include <imgui.h>
#include <thread>

#include "go_to_bin_dir.hpp"
#include "kengine.hpp"
#include "data/WindowComponent.hpp"
#include "functions/Execute.hpp"

#include "systems/sfml/SFMLSystem.hpp"

#include "helpers/MainLoop.hpp"

int main(int, char ** av) {
	putils::goToBinDir(av[0]);

#if defined(_WIN32) && defined(KENGINE_NDEBUG)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	kengine::init(std::thread::hardware_concurrency());
	kengine::entities += kengine::SFMLSystem();

	kengine::entities += [](kengine::Entity & e) {
		e += kengine::WindowComponent{
			.name = "Sandbox"
		};

		e += kengine::functions::Execute{
			[](float deltaTime) {
				if (ImGui::Begin("Zob")) {
				}
				ImGui::End();
			}
		};
	};

	kengine::mainLoop::timeModulated::run();

	return 0;
}
