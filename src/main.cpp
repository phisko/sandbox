#include <thread>

#include "go_to_bin_dir.hpp"
#include "kengine.hpp"
#include "PluginManager.hpp"

#include "data/CameraComponent.hpp"
#include "data/DebugGraphicsComponent.hpp"
#include "data/GraphicsComponent.hpp"
#include "data/TransformComponent.hpp"
#include "data/ViewportComponent.hpp"
#include "data/WindowComponent.hpp"

#include "functions/Execute.hpp"

#include "helpers/imguiHelper.hpp"
#include "helpers/registerTypeHelper.hpp"
#include "helpers/MainLoop.hpp"

#include "systems/imgui_adjustable/ImGuiAdjustableSystem.hpp"
#include "systems/imgui_entity_editor/ImGuiEntityEditorSystem.hpp"
#include "systems/imgui_entity_selector/ImGuiEntitySelectorSystem.hpp"
#include "systems/imgui_tool/ImGuiToolSystem.hpp"
#include "systems/log_stdout/LogStdoutSystem.hpp"
#include "systems/model_creator/ModelCreatorSystem.hpp"
#include "systems/sfml/SFMLSystem.hpp"

#include "meta/GenomeComponent.hpp"
#include "data/ReproductionComponent.hpp"

static void setupGenome() noexcept {
	using kengine::typeHelper::getTypeEntity;

	getTypeEntity<kengine::TransformComponent>() += meta::GenomeComponent{
		.attributes = {
			{
				.name = "boundingBox.size",
				.mutate = [](void * attribute, size_t, const meta::GenomeComponent::Mutator & mutator) noexcept {
					const auto size = (putils::Point3f *)attribute;
					size_t intSize[] = { (size_t)size->x, (size_t)size->y, (size_t)size->z };
					mutator(intSize, sizeof(intSize));
					for (size_t i = 0; i < 3; ++i)
						(*size)[i] = intSize[i] + std::floor((*size)[i]);
				}
			}
		}
	};

	getTypeEntity<kengine::GraphicsComponent>() += meta::GenomeComponent{
		.attributes = {
			{
				.name = "color",
				.mutate = [](void * attribute, size_t size, const meta::GenomeComponent::Mutator & mutator) noexcept {
					const auto color = (putils::NormalizedColor *)attribute;
					putils::Color mutatable(toColor(*color));
					mutator(&mutatable, sizeof(mutatable));
					*color = toNormalizedColor(mutatable);
				}
			}
		}
	};
}

static void Window(kengine::Entity & e) {
	e += kengine::NameComponent{ "Window" };
	e += kengine::WindowComponent{
		.name = "Sandbox"
	};
	e += kengine::CameraComponent{
		.frustum = {
			.size = {16.f, 9.f, 0.f}
		}
	};
	e += kengine::ViewportComponent{};
}

int main(int, char ** av) {
	putils::goToBinDir(av[0]);

#if defined(_WIN32) && defined(KENGINE_NDEBUG)
	ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif

	kengine::init(std::thread::hardware_concurrency());

	setupGenome();

	kengine::entities += kengine::ImGuiAdjustableSystem();
	kengine::entities += kengine::ImGuiEntityEditorSystem();
	kengine::entities += kengine::ImGuiEntitySelectorSystem();
	kengine::entities += kengine::ImGuiToolSystem();
	kengine::entities += kengine::LogStdoutSystem();

	kengine::entities += kengine::ModelCreatorSystem();
	kengine::entities += kengine::SFMLSystem();

	kengine::types::registerTypes();

	putils::PluginManager pm;
	pm.rescanDirectory("plugins", "loadKenginePlugin", kengine::getState());

	kengine::entities += Window;

	kengine::EntityID firstParentID;
	kengine::entities += [&](kengine::Entity & firstParent) {
		firstParentID = firstParent.id;

		firstParent += kengine::NameComponent{ "First parent" };
		firstParent += kengine::GraphicsComponent{
			.appearance = "resources/textures/character.png",
			.color = putils::NormalizedColor{1.f, 1.f, 0.f}
		};
		firstParent += kengine::TransformComponent{};
	};

	kengine::EntityID secondParentID;
	kengine::entities += [&](kengine::Entity & secondParent) {
		secondParentID = secondParent.id;

		secondParent += kengine::NameComponent{ "Second parent" };
		secondParent += kengine::GraphicsComponent{
			.appearance = "resources/textures/character.png",
			.color = putils::NormalizedColor{ 0.f, 0.5f, 0.5f }
		};
		secondParent += kengine::TransformComponent{
			.boundingBox = {
				.position = { 1.f, 1.f, 1.f },
				.size = { 1.f, 1.f, 1.f }
			}
		};
	};

	kengine::entities[firstParentID] += ReproductionComponent{ .mate = secondParentID };
	kengine::entities[secondParentID] += ReproductionComponent{ .mate = firstParentID };

	kengine::mainLoop::timeModulated::run();

	return 0;
}
